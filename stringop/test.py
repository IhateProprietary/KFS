#!/Users/jye/.brew/bin/python3

import os
import random
from enum import Enum
import sys
import getopt
import subprocess
import shlex

DEVRANDOM = "/dev/urandom"

class CTYPE(Enum):
    VOID = "void"
    CHAR = "char"
    SHORT = "short"
    INT = "int"
    LONG = "long int"
    UNSIGNED = "unsigned"
    SIZE_T = "size_t"

class DEF(Enum):
    MEMORY = "-D__TEST_MEMORY__"
    STRING = "-D__TEST_STRING__"
    RETURN = "-D__TEST_RETURN__"
    PTR = "-D__PTR__"
    ORIGINAL = "-D__ORIGINAL__"

class ARGV(Enum):
    DST = "{dst}"
    SRC = "src"
    SIZE = "bufsize"
    FD_READ = "0"
    INT = "{char}"
    CMP = "__cmp"

FUNCTION = {
    "memchr": {
        "type": [CTYPE.VOID, 1],
        "argv": [
            [CTYPE.VOID, ARGV.SRC, 1],
            [CTYPE.INT, ARGV.INT, 0],
            [CTYPE.SIZE_T, ARGV.SIZE, 0]
        ],
        "cdefines": [DEF.RETURN, DEF.ORIGINAL],
        "test_type": 0
    },
    "memcmp": {
        "type": [CTYPE.INT, 0],
        "argv": [
            [CTYPE.VOID, ARGV.SRC, 1],
            [CTYPE.VOID, ARGV.CMP, 1],
            [CTYPE.SIZE_T, ARGV.SIZE, 0]
        ],
        "cdefines": [DEF.RETURN, DEF.ORIGINAL],
        "test_type": 0
    },
    "memcpy": {
        "type": [CTYPE.VOID, 1],
        "argv": [
            [CTYPE.VOID, ARGV.DST, 1],
            [CTYPE.VOID, ARGV.SRC, 1],
            [CTYPE.SIZE_T, ARGV.SIZE, 0]
        ],
        "cdefines": [DEF.PTR, DEF.MEMORY, DEF.ORIGINAL],
        "test_type": 0
    },
    "memset": {
        "type": [CTYPE.VOID, 1],
        "argv": [
            [CTYPE.VOID, ARGV.DST, 1],
            [CTYPE.INT, ARGV.INT, 0],
            [CTYPE.SIZE_T, ARGV.SIZE, 0]
        ],
        "cdefines": [DEF.MEMORY, DEF.ORIGINAL],
        "test_type": 0
    },
    "strchr": {
        "type": [CTYPE.CHAR, 1],
        "argv": [
            [CTYPE.CHAR, ARGV.SRC, 1],
            [CTYPE.INT, ARGV.INT, 0]
        ],
        "cdefines": [DEF.RETURN, DEF.ORIGINAL],
        "test_type": 0
    },
    "strcmp": {
        "type": [CTYPE.INT, 0],
        "argv": [
            [CTYPE.CHAR, ARGV.SRC, 1],
            [CTYPE.CHAR, ARGV.CMP, 1]
        ],
        "cdefines": [DEF.RETURN, DEF.ORIGINAL],
        "test_type": 0
    },
    "strlen": {
        "type": [CTYPE.SIZE_T, 0],
        "argv": [
            [CTYPE.CHAR, ARGV.SRC, 1],
        ],
        "cdefines": [DEF.RETURN, DEF.ORIGINAL],
        "test_type": 0
    },
}

TEMPLATE="""#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#define ft_ _{func}

{_type} {ptr}ft_({argv});

int     main(int ac, char **av)
{{
    char src[] = "{buffer1}";
    size_t bufsize = sizeof(src);
    char *__dst = malloc(bufsize);
    char __cmp[] = "{buffer2}";
    int fd = open("/dev/urandom", O_RDONLY);
#ifdef __TEST_STRING__
    __cmp[{rdm}] = 0;
    src[{rdm}] = 0;
#endif

    assert(__dst != (char *)0);
    assert(fd != -1);
    read(fd, __dst, bufsize);
#ifdef __TEST_STRING__
    if({rdm2})
        memcpy(__dst, src, {rdm2});
    __dst[{rdm2}] = 0;
#endif

#ifdef __ORIGINAL__
    char *__odst = malloc(bufsize);

    assert(__odst != (char *)0);
    read(fd, __odst, bufsize);

# ifdef __TEST_STRING__
    if({rdm2})
        memcpy(__odst, src, {rdm2});
    __odst[{rdm2}] = 0;
# endif

# if defined(__TEST_RETURN__) || defined(__PTR__)
    {_type} {ptr}oret = {func}({oarg});
# else
    {func}({oarg});
# endif
#endif

#if defined(__TEST_RETURN__) || defined(__PTR__)
    {_type} {ptr}ret = ft_({arg});
#else
    ft_({arg});
#endif

#if defined(__ORIGINAL__) && defined(__TEST_MEMORY__)
# ifdef __PTR__
    assert(!memcmp(ret, src, bufsize));
# else
    assert(!memcmp(__odst, __dst, bufsize));
# endif
#endif

#if defined(__ORIGINAL__) && defined(__TEST_STRING__)
# ifdef __PTR__
    assert(!strcmp(ret, src));
# else
    assert(!strcmp(__odst, __dst));
# endif
#endif

#ifdef __TEST_RETURN__
    assert(ret == oret);
#endif
    return (0);
}}
"""

def format_func_prot(_type, _ptr):
    return _type.value + " " + "*"*_ptr

def format_func_argument(_argv_type):
    return _argv_type.value

if __name__ == "__main__":
    fd = os.open("/dev/urandom", os.O_RDONLY)
    optlist, arg = getopt.gnu_getopt(sys.argv, "p:s:n:", longopts=["suffix=", "prefix=", "number="])
    folder = "./"
    suffix = "_"
    prefix = "test_"
    n = 5
    for opt, _arg in optlist:
        if opt == "--suffix" or opt == "-s":
            suffix = _arg
        elif opt == "--prefix" or opt == "-p":
            prefix = _arg
        elif opt == "--number" or opt == "-n":
            n = int(_arg)

    for _ in range(0, n):
        for key, value in FUNCTION.items():
            argv = [
                (format_func_prot(_type, _ptr),
                 format_func_argument(_argv_type))
                for _type, _argv_type, _ptr in value["argv"]
            ]
            sizebuf = random.randrange(32, 16384+1, 8)
            ## get randomized buffer
            _buf1 = os.read(fd, sizebuf)
            _buf2 = os.read(fd, sizebuf)
            ## writing raw string in hexadecimal
            buf1 = "".join(["\\x{:02x}".format(x) for x in _buf1])
            buf2 = "".join(["\\x{:02x}".format(x) for x in _buf2])
            ## filename
            filename = folder+prefix+key+suffix+str(_)
            ## if string, put a 0 somewhere, assuming there isn't one already in /dev/urandom
            _cut = random.randrange(0, sizebuf, 8)
            _cut2 = random.randrange(0, sizebuf - _cut, 8);
            z = os.open(filename + ".c", os.O_TRUNC | os.O_CREAT | os.O_WRONLY, 0o644)
            ## if testing with an int
            rchar=random.randrange(1, 127)
            ## outputing format template in a file
            os.write(z, TEMPLATE.format(
                func=key,
                _type = value["type"][0].value,
                ptr = value["type"][1] * "*",
                argv=", ".join([x[0] for x in argv]),
                arg=", ".join([x[1] for x in argv]).format(dst = "__dst", char = rchar),
                oarg=", ".join([x[1] for x in argv]).format(dst = "__odst", char = rchar),
                rdm = _cut,
                rdm2 = _cut2,
                buffer1=buf1,
                buffer2=buf2,
            ).encode("UTF-8"))
            os.close(z)
            cmd = "gcc {filename}.c -o {filename} {DEFINES} -g3 -L. -lsops".format(filename=filename, DEFINES=" ".join([x.value for x in value["cdefines"]]))
            cmd = shlex.split(cmd)
            subprocess.run(cmd)
            ## testing against original
            if value["test_type"] == 0:
                _ret = subprocess.run(filename, capture_output=True)
                if _ret.returncode != 0:
                    print(filename + " KO")
                else:
                    print(filename + " OK")
            ## testing output against our own, assuming below is a string
            elif value["test_type"] == 1:
                _ret = subprocess.Popen(filename, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
                ret = _ret.communicate(input=_buf1)[0]
                __cut = _buf1.find(0)
                if __cut != -1:
                    _cut = _cut if _cut < __cut else __cut
                if ret != _buf1[:_cut]:
                    _fd = os.open(key+str(_)+"_output", os.O_CREAT | os.O_WRONLY, 0o644)
                    os.write(_fd, ret)
                    os.close(_fd)
                    _fd = os.open(key+str(_)+"original_output", os.O_CREAT | os.O_WRONLY, 0o644)
                    os.write(_fd, _buf1[:_cut])
                    os.close(_fd)
                    print(filename + " KO")
                else:
                    print(filename + " OK")
            ## testing output against our own, assuming it read everything including '\0'
            elif value["test_type"] == 2:
                _ret = subprocess.Popen(filename, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
                ret = _ret.communicate(input=_buf1)[0]
                if ret != _buf1:
                    print(filename + " KO")
                else:
                    print(filename + " OK")
