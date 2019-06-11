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

#ifndef __ACPI_H__
# define __ACPI_H__ 1

# include "xstdint.h"

/*
 * These data can be found in the multiboot boot information
 */
struct acpi_rsdp
{
	u8 signature[8];
	u8 chksum;
	u8 oemid[6];
	/* rev == 0 if acpi == v1 */
	/* rev == 2 if acpi == v2 */
	u8 rev;
	u32 sdt_p;
} __attribute__((packed));

/* 64 bit extended */
struct acpi_rsdp_2
{
	struct acpi_rsdp rsdp;
	u32 len;
	u32 xsdt;
	u8 xchksum;
	u8 reserved[3];
} __attribute__((packed));

/*
 * Specified in ACPI 1.0b Manual
 */
struct acpi_sdt_header
{
	u8 signature[4];
	u32 len;
	u8 rev;
	u8 chksum;
	u8 oemid[6];
	u8 oemtableid[8];
	u32 oemrev;
	u32 creator_id;
	u32 creator_rev;
} __attribute__((packed));

struct acpi_ptr_table
{
	struct acpi_sdt_header hdr; /* signature RSDT */
	u32 sdt_p[0]; /* length is determined by hdr.len - sizeof hdr */
} __attribute__((packed));

struct acpi_sdt_facp
{
	struct acpi_sdt_header hdr; /* signature FACP*/
	u32 facs_addr; /* facs structure addr */
	u32 dsdt_addr; /* AML Firmware addr */
	u8 reserved1;
	u8 pref_profile;
	u16 sci_int;
	u32 smi_cmd;

	u8 acpi_enable;
	u8 acpi_disable;

	u8 s4bios_req;
	u8 pstate_cnt;

	u32 pm1a_evt_blk;
	u32 pm1b_evt_blk;
	u32 pm1a_cnt_blk;
	u32 pm1b_cnt_blk;
	u32 pm2_cnt_blk;
	u32 pm_tmr_blk;

	u8 gpe0_blk;
	u8 gpe1_blk;
	u8 gpe1_base;
	u8 cst_cnt;

	u16 p_lvl2_lat;
	u16 p_lvl3_lat;

	u16 flush_size;
	u16 flush_stride;
	u8 duty_offset;
	u8 duty_width;
	u8 dayalarm;
	u8 monthalarm;
	u8 century;

	u8 reserved2[3];

	u32 flags;
} __attribute__((packed));

#define FACS_STRUCT_ALIGN 0x40
struct acpi_sdt_facs
{
	u8 signature[4]; /* signature FACS */
	u32 length;
	u32 hw_sign;
	u32 fw_waking_vector;
	u32 global_lock;
#define S4BIOS_F 1 /*specify if S4BIOS is supported*/
	u32 flags;
} __attribute__((packed));

/*
 * Below are APIC specific structure
 * to get I/O APIC and LAPIC addresses
 */

struct acpi_sdt_apic
{
	struct acpi_sdt_header hdr;
	u32 lapic_addr;
#define PCAT_COMPAT 01 /* indicates compatible dual-8259 setup */
	u32 flags;
	u8 apic_info[0];
} __attribute__((packed));

struct acpi_sdt_apic_type
{
	u8 type;
	u8 length;
} __attribute__((packed));

struct acpi_sdt_lapic
{
#define ACPI_APIC_TYPE_LOCAL 0
	u8 type;
	u8 length; /* == 8 */
	u8 acpi_proc_id;
	u8 lapic_id;
#define ACPI_LAPIC_ENABLED 1
	u32 flags;
} __attribute__((packed));

struct acpi_sdt_ioapic
{
#define ACPI_APIC_TYPE_IO 1
	u8 type;
	u8 length; /* == 12*/
	u8 io_apic_id;
	u8 reserved;
	u32 ioapic_addr;
	u32 sys_vector_base;
} __attribute__((packed));

#endif /* __ACPI_H__ */
