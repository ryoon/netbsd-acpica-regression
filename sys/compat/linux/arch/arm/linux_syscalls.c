/* $NetBSD: linux_syscalls.c,v 1.57 2013/12/08 15:56:12 njoly Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.54 2013/12/08 15:55:10 njoly Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: linux_syscalls.c,v 1.57 2013/12/08 15:56:12 njoly Exp $");

#if defined(_KERNEL_OPT)
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <sys/time.h>
#include <compat/sys/time.h>
#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_siginfo.h>
#include <compat/linux/common/linux_machdep.h>
#include <compat/linux/linux_syscallargs.h>
#endif /* _KERNEL_OPT */

const char *const linux_syscallnames[] = {
	/*   0 */	"nosys",
	/*   1 */	"exit",
	/*   2 */	"fork",
	/*   3 */	"read",
	/*   4 */	"write",
	/*   5 */	"open",
	/*   6 */	"close",
	/*   7 */	"waitpid",
	/*   8 */	"creat",
	/*   9 */	"link",
	/*  10 */	"unlink",
	/*  11 */	"execve",
	/*  12 */	"chdir",
	/*  13 */	"time",
	/*  14 */	"mknod",
	/*  15 */	"chmod",
	/*  16 */	"lchown16",
	/*  17 */	"#17 (obsolete break)",
	/*  18 */	"#18 (obsolete ostat)",
	/*  19 */	"lseek",
	/*  20 */	"getpid",
	/*  21 */	"#21 (unimplemented mount)",
	/*  22 */	"#22 (obsolete umount)",
	/*  23 */	"setuid",
	/*  24 */	"getuid",
	/*  25 */	"stime",
	/*  26 */	"ptrace",
	/*  27 */	"alarm",
	/*  28 */	"#28 (obsolete ofstat)",
	/*  29 */	"pause",
	/*  30 */	"utime",
	/*  31 */	"#31 (obsolete stty)",
	/*  32 */	"#32 (obsolete gtty)",
	/*  33 */	"access",
	/*  34 */	"nice",
	/*  35 */	"#35 (obsolete ftime)",
	/*  36 */	"sync",
	/*  37 */	"kill",
	/*  38 */	"__posix_rename",
	/*  39 */	"mkdir",
	/*  40 */	"rmdir",
	/*  41 */	"dup",
	/*  42 */	"pipe",
	/*  43 */	"times",
	/*  44 */	"#44 (obsolete prof)",
	/*  45 */	"brk",
	/*  46 */	"setgid",
	/*  47 */	"getgid",
	/*  48 */	"signal",
	/*  49 */	"geteuid",
	/*  50 */	"getegid",
	/*  51 */	"acct",
	/*  52 */	"#52 (unimplemented umount)",
	/*  53 */	"#53 (obsolete lock)",
	/*  54 */	"ioctl",
	/*  55 */	"fcntl",
	/*  56 */	"#56 (obsolete mpx)",
	/*  57 */	"setpgid",
	/*  58 */	"#58 (obsolete ulimit)",
	/*  59 */	"oldolduname",
	/*  60 */	"umask",
	/*  61 */	"chroot",
	/*  62 */	"#62 (unimplemented ustat)",
	/*  63 */	"dup2",
	/*  64 */	"getppid",
	/*  65 */	"getpgrp",
	/*  66 */	"setsid",
	/*  67 */	"sigaction",
	/*  68 */	"siggetmask",
	/*  69 */	"sigsetmask",
	/*  70 */	"setreuid16",
	/*  71 */	"setregid16",
	/*  72 */	"sigsuspend",
	/*  73 */	"sigpending",
	/*  74 */	"sethostname",
	/*  75 */	"setrlimit",
	/*  76 */	"getrlimit",
	/*  77 */	"getrusage",
	/*  78 */	"gettimeofday",
	/*  79 */	"settimeofday",
	/*  80 */	"getgroups16",
	/*  81 */	"setgroups16",
	/*  82 */	"oldselect",
	/*  83 */	"symlink",
	/*  84 */	"oolstat",
	/*  85 */	"readlink",
#ifdef EXEC_AOUT
	/*  86 */	"uselib",
#else
	/*  86 */	"#86 (excluded uselib)",
#endif
	/*  87 */	"swapon",
	/*  88 */	"reboot",
	/*  89 */	"readdir",
	/*  90 */	"old_mmap",
	/*  91 */	"munmap",
	/*  92 */	"truncate",
	/*  93 */	"ftruncate",
	/*  94 */	"fchmod",
	/*  95 */	"fchown16",
	/*  96 */	"getpriority",
	/*  97 */	"setpriority",
	/*  98 */	"profil",
	/*  99 */	"statfs",
	/* 100 */	"fstatfs",
	/* 101 */	"#101 (unimplemented)",
	/* 102 */	"socketcall",
	/* 103 */	"#103 (unimplemented syslog)",
	/* 104 */	"setitimer",
	/* 105 */	"getitimer",
	/* 106 */	"stat",
	/* 107 */	"lstat",
	/* 108 */	"fstat",
	/* 109 */	"olduname",
	/* 110 */	"#110 (unimplemented)",
	/* 111 */	"#111 (unimplemented vhangup)",
	/* 112 */	"#112 (unimplemented idle)",
	/* 113 */	"#113 (unimplemented syscall)",
	/* 114 */	"wait4",
	/* 115 */	"swapoff",
	/* 116 */	"sysinfo",
	/* 117 */	"ipc",
	/* 118 */	"fsync",
	/* 119 */	"sigreturn",
	/* 120 */	"clone",
	/* 121 */	"setdomainname",
	/* 122 */	"uname",
	/* 123 */	"#123 (unimplemented modify_ldt)",
	/* 124 */	"#124 (unimplemented adjtimex)",
	/* 125 */	"mprotect",
	/* 126 */	"sigprocmask",
	/* 127 */	"#127 (unimplemented create_module)",
	/* 128 */	"#128 (unimplemented init_module)",
	/* 129 */	"#129 (unimplemented delete_module)",
	/* 130 */	"#130 (unimplemented get_kernel_syms)",
	/* 131 */	"#131 (unimplemented quotactl)",
	/* 132 */	"getpgid",
	/* 133 */	"fchdir",
	/* 134 */	"#134 (unimplemented bdflush)",
	/* 135 */	"#135 (unimplemented sysfs)",
	/* 136 */	"personality",
	/* 137 */	"#137 (unimplemented afs_syscall)",
	/* 138 */	"setfsuid",
	/* 139 */	"setfsgid",
	/* 140 */	"llseek",
	/* 141 */	"getdents",
	/* 142 */	"select",
	/* 143 */	"flock",
	/* 144 */	"__msync13",
	/* 145 */	"readv",
	/* 146 */	"writev",
	/* 147 */	"getsid",
	/* 148 */	"fdatasync",
	/* 149 */	"__sysctl",
	/* 150 */	"mlock",
	/* 151 */	"munlock",
	/* 152 */	"mlockall",
	/* 153 */	"munlockall",
	/* 154 */	"sched_setparam",
	/* 155 */	"sched_getparam",
	/* 156 */	"sched_setscheduler",
	/* 157 */	"sched_getscheduler",
	/* 158 */	"sched_yield",
	/* 159 */	"sched_get_priority_max",
	/* 160 */	"sched_get_priority_min",
	/* 161 */	"#161 (unimplemented sched_rr_get_interval)",
	/* 162 */	"nanosleep",
	/* 163 */	"mremap",
	/* 164 */	"setresuid16",
	/* 165 */	"getresuid16",
	/* 166 */	"#166 (unimplemented)",
	/* 167 */	"#167 (unimplemented query_module)",
	/* 168 */	"poll",
	/* 169 */	"#169 (unimplemented nfsservctl)",
	/* 170 */	"setresgid16",
	/* 171 */	"getresgid16",
	/* 172 */	"#172 (unimplemented prctl)",
	/* 173 */	"#173 (unimplemented rt_sigreturn)",
	/* 174 */	"rt_sigaction",
	/* 175 */	"rt_sigprocmask",
	/* 176 */	"rt_sigpending",
	/* 177 */	"rt_sigtimedwait",
	/* 178 */	"rt_queueinfo",
	/* 179 */	"rt_sigsuspend",
	/* 180 */	"pread",
	/* 181 */	"pwrite",
	/* 182 */	"chown16",
	/* 183 */	"__getcwd",
	/* 184 */	"#184 (unimplemented capget)",
	/* 185 */	"#185 (unimplemented capset)",
	/* 186 */	"sigaltstack",
	/* 187 */	"#187 (unimplemented sendfile)",
	/* 188 */	"#188 (unimplemented getpmsg)",
	/* 189 */	"#189 (unimplemented putpmsg)",
	/* 190 */	"vfork",
	/* 191 */	"ugetrlimit",
#define linux_sys_mmap2_args linux_sys_mmap_args
	/* 192 */	"mmap2",
	/* 193 */	"truncate64",
	/* 194 */	"ftruncate64",
	/* 195 */	"stat64",
	/* 196 */	"lstat64",
	/* 197 */	"fstat64",
	/* 198 */	"__posix_lchown",
	/* 199 */	"getuid32",
	/* 200 */	"getgid32",
	/* 201 */	"geteuid32",
	/* 202 */	"getegid32",
	/* 203 */	"setreuid32",
	/* 204 */	"setregid32",
	/* 205 */	"getgroups32",
	/* 206 */	"setgroups32",
	/* 207 */	"fchown32",
	/* 208 */	"setresuid32",
	/* 209 */	"getresuid32",
	/* 210 */	"setresgid32",
	/* 211 */	"getresgid32",
	/* 212 */	"chown32",
	/* 213 */	"setuid32",
	/* 214 */	"setgid32",
	/* 215 */	"setfsuid32",
	/* 216 */	"setfsgid32",
	/* 217 */	"getdents64",
	/* 218 */	"#218 (unimplemented pivot_root)",
	/* 219 */	"mincore",
	/* 220 */	"madvise",
	/* 221 */	"fcntl64",
	/* 222 */	"#222 (unimplemented / * for tux * /)",
	/* 223 */	"#223 (unimplemented / * unused * /)",
	/* 224 */	"gettid",
	/* 225 */	"#225 (unimplemented readahead)",
	/* 226 */	"setxattr",
	/* 227 */	"lsetxattr",
	/* 228 */	"fsetxattr",
	/* 229 */	"getxattr",
	/* 230 */	"lgetxattr",
	/* 231 */	"fgetxattr",
	/* 232 */	"listxattr",
	/* 233 */	"llistxattr",
	/* 234 */	"flistxattr",
	/* 235 */	"removexattr",
	/* 236 */	"lremovexattr",
	/* 237 */	"fremovexattr",
	/* 238 */	"tkill",
	/* 239 */	"#239 (unimplemented sendfile64)",
	/* 240 */	"futex",
	/* 241 */	"sched_setaffinity",
	/* 242 */	"sched_getaffinity",
	/* 243 */	"#243 (unimplemented io_setup)",
	/* 244 */	"#244 (unimplemented io_destroy)",
	/* 245 */	"#245 (unimplemented io_getevents)",
	/* 246 */	"#246 (unimplemented io_submit)",
	/* 247 */	"#247 (unimplemented io_cancel)",
	/* 248 */	"exit_group",
	/* 249 */	"#249 (unimplemented lookup_dcookie)",
	/* 250 */	"#250 (unimplemented epoll_create)",
	/* 251 */	"#251 (unimplemented epoll_ctl)",
	/* 252 */	"#252 (unimplemented epoll_wait)",
	/* 253 */	"#253 (unimplemented remap_file_pages)",
	/* 254 */	"#254 (unimplemented set_thread_area)",
	/* 255 */	"#255 (unimplemented get_thread_area)",
	/* 256 */	"set_tid_address",
	/* 257 */	"#257 (unimplemented timer_create)",
	/* 258 */	"#258 (unimplemented timer_settime)",
	/* 259 */	"#259 (unimplemented timer_gettime)",
	/* 260 */	"#260 (unimplemented timer_getoverrun)",
	/* 261 */	"#261 (unimplemented timer_delete)",
	/* 262 */	"clock_settime",
	/* 263 */	"clock_gettime",
	/* 264 */	"clock_getres",
	/* 265 */	"clock_nanosleep",
	/* 266 */	"statfs64",
	/* 267 */	"fstatfs64",
	/* 268 */	"tgkill",
	/* 269 */	"utimes",
	/* 270 */	"fadvise64_64",
	/* 271 */	"#271 (unimplemented pciconfig_iobase)",
	/* 272 */	"#272 (unimplemented pciconfig_read)",
	/* 273 */	"#273 (unimplemented pciconfig_write)",
	/* 274 */	"#274 (unimplemented mq_open)",
	/* 275 */	"#275 (unimplemented mq_unlink)",
	/* 276 */	"#276 (unimplemented mq_timedsend)",
	/* 277 */	"#277 (unimplemented mq_timedreceive)",
	/* 278 */	"#278 (unimplemented mq_notify)",
	/* 279 */	"#279 (unimplemented mq_getsetattr)",
	/* 280 */	"#280 (unimplemented waitid)",
	/* 281 */	"#281 (unimplemented socket)",
	/* 282 */	"#282 (unimplemented bind)",
	/* 283 */	"#283 (unimplemented connect)",
	/* 284 */	"#284 (unimplemented listen)",
	/* 285 */	"#285 (unimplemented accept)",
	/* 286 */	"#286 (unimplemented getsockname)",
	/* 287 */	"#287 (unimplemented getpeername)",
	/* 288 */	"#288 (unimplemented socketpair)",
	/* 289 */	"#289 (unimplemented send)",
	/* 290 */	"#290 (unimplemented sendto)",
	/* 291 */	"#291 (unimplemented recv)",
	/* 292 */	"#292 (unimplemented recvfrom)",
	/* 293 */	"#293 (unimplemented shutdown)",
	/* 294 */	"#294 (unimplemented setsockopt)",
	/* 295 */	"#295 (unimplemented getsockopt)",
	/* 296 */	"#296 (unimplemented sendmsg)",
	/* 297 */	"#297 (unimplemented recvmsg)",
	/* 298 */	"#298 (unimplemented semop)",
	/* 299 */	"#299 (unimplemented semget)",
	/* 300 */	"#300 (unimplemented semctl)",
	/* 301 */	"#301 (unimplemented msgsnd)",
	/* 302 */	"#302 (unimplemented msgrcv)",
	/* 303 */	"#303 (unimplemented msgget)",
	/* 304 */	"#304 (unimplemented msgctl)",
	/* 305 */	"#305 (unimplemented shmat)",
	/* 306 */	"#306 (unimplemented shmdt)",
	/* 307 */	"#307 (unimplemented shmget)",
	/* 308 */	"#308 (unimplemented shmctl)",
	/* 309 */	"#309 (unimplemented add_key)",
	/* 310 */	"#310 (unimplemented request_key)",
	/* 311 */	"#311 (unimplemented keyctl)",
	/* 312 */	"#312 (unimplemented semtimedop)",
	/* 313 */	"#313 (unimplemented vserver)",
	/* 314 */	"#314 (unimplemented ioptio_set)",
	/* 315 */	"#315 (unimplemented ioptio_get)",
	/* 316 */	"#316 (unimplemented inotify_init)",
	/* 317 */	"#317 (unimplemented inotify_add_watch)",
	/* 318 */	"#318 (unimplemented inotify_rm_watch)",
	/* 319 */	"#319 (unimplemented mbind)",
	/* 320 */	"#320 (unimplemented get_mempolicy)",
	/* 321 */	"#321 (unimplemented set_mempolicy)",
	/* 322 */	"openat",
	/* 323 */	"mkdirat",
	/* 324 */	"mknodat",
	/* 325 */	"fchownat",
	/* 326 */	"#326 (unimplemented futimesat)",
	/* 327 */	"fstatat64",
	/* 328 */	"unlinkat",
	/* 329 */	"renameat",
	/* 330 */	"linkat",
	/* 331 */	"symlinkat",
	/* 332 */	"readlinkat",
	/* 333 */	"fchmodat",
	/* 334 */	"faccessat",
	/* 335 */	"#335 (unimplemented pselect6)",
	/* 336 */	"ppoll",
	/* 337 */	"#337 (unimplemented unshare)",
	/* 338 */	"set_robust_list",
	/* 339 */	"get_robust_list",
	/* 340 */	"#340 (unimplemented splice)",
	/* 341 */	"#341 (unimplemented sync_file_range2)",
	/* 342 */	"#342 (unimplemented tee)",
	/* 343 */	"#343 (unimplemented vmsplice)",
	/* 344 */	"#344 (unimplemented move_pages)",
	/* 345 */	"#345 (unimplemented getcpu)",
	/* 346 */	"#346 (unimplemented epoll_wait)",
	/* 347 */	"#347 (unimplemented kexec_load)",
	/* 348 */	"utimensat",
	/* 349 */	"#349 (unimplemented signalfd)",
	/* 350 */	"#350 (unimplemented timerfd_create)",
	/* 351 */	"#351 (unimplemented eventfd)",
	/* 352 */	"#352 (unimplemented fallocate)",
	/* 353 */	"#353 (unimplemented timerfd_settime)",
	/* 354 */	"#354 (unimplemented timerfd_gettime)",
	/* 355 */	"#355 (unimplemented signalfd4)",
	/* 356 */	"#356 (unimplemented eventfd2)",
	/* 357 */	"#357 (unimplemented epoll_create1)",
	/* 358 */	"dup3",
	/* 359 */	"pipe2",
	/* 360 */	"#360 (unimplemented inotify_init1)",
	/* 361 */	"#361 (unimplemented preadv)",
	/* 362 */	"#362 (unimplemented pwritev)",
	/* 363 */	"#363 (unimplemented rt_tgsigqueueinfo)",
	/* 364 */	"#364 (unimplemented perf_counter_open)",
	/* 365 */	"#365 (unimplemented recvmmsg)",
	/* 366 */	"#366 (unimplemented)",
	/* 367 */	"#367 (unimplemented)",
	/* 368 */	"#368 (unimplemented)",
	/* 369 */	"#369 (unimplemented)",
	/* 370 */	"#370 (unimplemented)",
	/* 371 */	"#371 (unimplemented)",
	/* 372 */	"#372 (unimplemented)",
	/* 373 */	"#373 (unimplemented)",
	/* 374 */	"#374 (unimplemented)",
	/* 375 */	"#375 (unimplemented)",
	/* 376 */	"#376 (unimplemented)",
	/* 377 */	"#377 (unimplemented)",
	/* 378 */	"#378 (unimplemented)",
	/* 379 */	"#379 (unimplemented)",
	/* 380 */	"#380 (unimplemented)",
	/* 381 */	"#381 (unimplemented)",
	/* 382 */	"#382 (unimplemented)",
	/* 383 */	"#383 (unimplemented)",
	/* 384 */	"#384 (unimplemented / * base * /)",
	/* 385 */	"breakpoint",
	/* 386 */	"cacheflush",
	/* 387 */	"#387 (unimplemented usr26)",
	/* 388 */	"#388 (unimplemented usr32)",
	/* 389 */	"set_tls",
	/* 390 */	"# filler",
	/* 391 */	"# filler",
	/* 392 */	"# filler",
	/* 393 */	"# filler",
	/* 394 */	"# filler",
	/* 395 */	"# filler",
	/* 396 */	"# filler",
	/* 397 */	"# filler",
	/* 398 */	"# filler",
	/* 399 */	"# filler",
	/* 400 */	"# filler",
	/* 401 */	"# filler",
	/* 402 */	"# filler",
	/* 403 */	"# filler",
	/* 404 */	"# filler",
	/* 405 */	"# filler",
	/* 406 */	"# filler",
	/* 407 */	"# filler",
	/* 408 */	"# filler",
	/* 409 */	"# filler",
	/* 410 */	"# filler",
	/* 411 */	"# filler",
	/* 412 */	"# filler",
	/* 413 */	"# filler",
	/* 414 */	"# filler",
	/* 415 */	"# filler",
	/* 416 */	"# filler",
	/* 417 */	"# filler",
	/* 418 */	"# filler",
	/* 419 */	"# filler",
	/* 420 */	"# filler",
	/* 421 */	"# filler",
	/* 422 */	"# filler",
	/* 423 */	"# filler",
	/* 424 */	"# filler",
	/* 425 */	"# filler",
	/* 426 */	"# filler",
	/* 427 */	"# filler",
	/* 428 */	"# filler",
	/* 429 */	"# filler",
	/* 430 */	"# filler",
	/* 431 */	"# filler",
	/* 432 */	"# filler",
	/* 433 */	"# filler",
	/* 434 */	"# filler",
	/* 435 */	"# filler",
	/* 436 */	"# filler",
	/* 437 */	"# filler",
	/* 438 */	"# filler",
	/* 439 */	"# filler",
	/* 440 */	"# filler",
	/* 441 */	"# filler",
	/* 442 */	"# filler",
	/* 443 */	"# filler",
	/* 444 */	"# filler",
	/* 445 */	"# filler",
	/* 446 */	"# filler",
	/* 447 */	"# filler",
	/* 448 */	"# filler",
	/* 449 */	"# filler",
	/* 450 */	"# filler",
	/* 451 */	"# filler",
	/* 452 */	"# filler",
	/* 453 */	"# filler",
	/* 454 */	"# filler",
	/* 455 */	"# filler",
	/* 456 */	"# filler",
	/* 457 */	"# filler",
	/* 458 */	"# filler",
	/* 459 */	"# filler",
	/* 460 */	"# filler",
	/* 461 */	"# filler",
	/* 462 */	"# filler",
	/* 463 */	"# filler",
	/* 464 */	"# filler",
	/* 465 */	"# filler",
	/* 466 */	"# filler",
	/* 467 */	"# filler",
	/* 468 */	"# filler",
	/* 469 */	"# filler",
	/* 470 */	"# filler",
	/* 471 */	"# filler",
	/* 472 */	"# filler",
	/* 473 */	"# filler",
	/* 474 */	"# filler",
	/* 475 */	"# filler",
	/* 476 */	"# filler",
	/* 477 */	"# filler",
	/* 478 */	"# filler",
	/* 479 */	"# filler",
	/* 480 */	"# filler",
	/* 481 */	"# filler",
	/* 482 */	"# filler",
	/* 483 */	"# filler",
	/* 484 */	"# filler",
	/* 485 */	"# filler",
	/* 486 */	"# filler",
	/* 487 */	"# filler",
	/* 488 */	"# filler",
	/* 489 */	"# filler",
	/* 490 */	"# filler",
	/* 491 */	"# filler",
	/* 492 */	"# filler",
	/* 493 */	"# filler",
	/* 494 */	"# filler",
	/* 495 */	"# filler",
	/* 496 */	"# filler",
	/* 497 */	"# filler",
	/* 498 */	"# filler",
	/* 499 */	"# filler",
	/* 500 */	"# filler",
	/* 501 */	"# filler",
	/* 502 */	"# filler",
	/* 503 */	"# filler",
	/* 504 */	"# filler",
	/* 505 */	"# filler",
	/* 506 */	"# filler",
	/* 507 */	"# filler",
	/* 508 */	"# filler",
	/* 509 */	"# filler",
	/* 510 */	"# filler",
	/* 511 */	"# filler",
};
