	.file	"common.c"
.globl mc_stats
	.data
	.align 4
	.type	mc_stats, @object
	.size	mc_stats, 4
mc_stats:
	.long	1
	.section	.rodata
.LC1:
	.string	"common.c"
	.align 8
.LC2:
	.string	"(res_mark_nr >= 0) && (res_mark_nr < 5)"
	.align 8
.LC3:
	.string	"Statistics for common resources (%s):\n"
.LC4:
	.string	"  - Time   : %.2f seconds\n"
.LC5:
	.string	"error: "
