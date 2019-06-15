/*
 *  This file is part of Kernel From Scratch.
 *
 *	Kernel From Scratch is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "multiboot.h"
#include "acpi.h"
#include "kernel.h"
#include "xstdint.h"
#include "minimal_stringop.h"

#define RSDT_SIGNATURE_LENGTH 4

typedef struct multiboot_tag mb2_tag_t;

void _set_sysinfo(void *restrict multiboot_info, struct _sysinfo *sys)
{
	u32 offset;
	u32 tag_size;
	const mb2_tag_t *tag;
	const u32 total_size = ((struct multiboot_tag_size *)	\
				multiboot_info)->total_size;

	for (offset = sizeof(struct multiboot_tag_size);
	     offset < total_size;
	     offset += tag_size) {
		tag = (mb2_tag_t *)((char *)multiboot_info + offset);
		tag_size = tag->size + (-tag->size & (MULTIBOOT_TAG_ALIGN - 1));

		switch (tag->type) {
		case MULTIBOOT_TAG_TYPE_ACPI_OLD:
		case MULTIBOOT_TAG_TYPE_ACPI_NEW:
			sys->acpi_rsdp = (u32)tag;  break;
		case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			sys->meminfo = (u32)tag;    break;
		case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
			sys->frameinfo = (u32)tag;  break;
		case MULTIBOOT_TAG_TYPE_MMAP:
			sys->mmap = (u32)tag;       break;
		case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
			sys->elfsection = (u32)tag; break;
		default: break;
		}
	}
}

void _set_apic_addr(struct acpi_sdt_apic *apic, struct _sysinfo *sys)
{
	const size_t size = apic->hdr.len;
	struct acpi_sdt_apic_type *p;
	size_t offset;

	(void)sys;

	printk("%.4s apic base addr 0x%x\n", apic->hdr.signature, apic);
	sys->lapic_addr = apic->lapic_addr;
	for (offset = sizeof *apic;
	     offset < size;
	     offset += p->length) {
		p = (struct acpi_sdt_apic_type *)((char *)apic + offset);
		switch (p->type) {
		case ACPI_APIC_TYPE_LOCAL:
			printk("flag lapic 0x%x id 0x%x\n",
			       ((struct acpi_sdt_lapic *)p)->flags,
			       ((struct acpi_sdt_lapic *)p)->lapic_id);
			break ;
		case ACPI_APIC_TYPE_IO:
			sys->ioapic_addr =
				((struct acpi_sdt_ioapic *)p)->ioapic_addr;
			printk("IOAPIC addr 0x%x\n", sys->ioapic_addr);
			break ;
		}
	}
}

int __rsdp_ok(struct multiboot_tag_acpi *rsdp)
{
	register const u8 *p = rsdp->rsdp;
	const u8 *end = rsdp->rsdp + sizeof(struct acpi_rsdp);
	u32 sum = 0;

	if (__builtin_expect(p > end, 0))
		end = (u8 *)~0UL;

	if (_memcmp("RSD PTR ", p, 8))
		return 1;
	for (; p < end; ++p)
		sum += *p;
	return (sum & 0xff);
}

int __checksum_ok(const u8 *restrict p, size_t size)
{
	register const u8 *end = p + size;
	u32 sum = 0;

	if (__builtin_expect(p > end, 0))
		end = (u8 *)~0UL;

	for (; p < end; ++p)
		sum += *p;
	return (sum & 0xff);
}

void *_find_sdt(void *root_sdp, const char *sign)
{
	const struct acpi_ptr_table *sdt_p = root_sdp;
	struct acpi_sdt_header *sdt;
	const size_t plen = (sdt_p->hdr.len - sizeof(struct acpi_sdt_header))
		/ sizeof(u32);
	size_t pcur = 0;

	for (; pcur < plen; pcur++) {
		sdt = (struct acpi_sdt_header *)sdt_p->sdt_p[pcur];
		if (!_memcmp(sign, sdt->signature, 4))
			return (void *)sdt;
	}
	return (0);
}
