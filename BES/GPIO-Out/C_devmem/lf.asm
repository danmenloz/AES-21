
LEDflash:     file format elf32-littlearm


Disassembly of section .init:

0001044c <_init>:
   1044c:	e92d4008 	push	{r3, lr}
   10450:	eb00003b 	bl	10544 <call_weak_fn>
   10454:	e8bd8008 	pop	{r3, pc}

Disassembly of section .plt:

00010458 <.plt>:
   10458:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
   1045c:	e59fe004 	ldr	lr, [pc, #4]	; 10468 <.plt+0x10>
   10460:	e08fe00e 	add	lr, pc, lr
   10464:	e5bef008 	ldr	pc, [lr, #8]!
   10468:	00010b98 	.word	0x00010b98

0001046c <printf@plt>:
   1046c:	e28fc600 	add	ip, pc, #0, 12
   10470:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   10474:	e5bcfb98 	ldr	pc, [ip, #2968]!	; 0xb98

00010478 <getuid@plt>:
   10478:	e28fc600 	add	ip, pc, #0, 12
   1047c:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   10480:	e5bcfb90 	ldr	pc, [ip, #2960]!	; 0xb90

00010484 <puts@plt>:
   10484:	e28fc600 	add	ip, pc, #0, 12
   10488:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   1048c:	e5bcfb88 	ldr	pc, [ip, #2952]!	; 0xb88

00010490 <__libc_start_main@plt>:
   10490:	e28fc600 	add	ip, pc, #0, 12
   10494:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   10498:	e5bcfb80 	ldr	pc, [ip, #2944]!	; 0xb80

0001049c <strerror@plt>:
   1049c:	e28fc600 	add	ip, pc, #0, 12
   104a0:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104a4:	e5bcfb78 	ldr	pc, [ip, #2936]!	; 0xb78

000104a8 <__gmon_start__@plt>:
   104a8:	e28fc600 	add	ip, pc, #0, 12
   104ac:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104b0:	e5bcfb70 	ldr	pc, [ip, #2928]!	; 0xb70

000104b4 <open@plt>:
   104b4:	e28fc600 	add	ip, pc, #0, 12
   104b8:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104bc:	e5bcfb68 	ldr	pc, [ip, #2920]!	; 0xb68

000104c0 <mmap@plt>:
   104c0:	e28fc600 	add	ip, pc, #0, 12
   104c4:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104c8:	e5bcfb60 	ldr	pc, [ip, #2912]!	; 0xb60

000104cc <getpagesize@plt>:
   104cc:	e28fc600 	add	ip, pc, #0, 12
   104d0:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104d4:	e5bcfb58 	ldr	pc, [ip, #2904]!	; 0xb58

000104d8 <__errno_location@plt>:
   104d8:	e28fc600 	add	ip, pc, #0, 12
   104dc:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104e0:	e5bcfb50 	ldr	pc, [ip, #2896]!	; 0xb50

000104e4 <__isoc99_sscanf@plt>:
   104e4:	e28fc600 	add	ip, pc, #0, 12
   104e8:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104ec:	e5bcfb48 	ldr	pc, [ip, #2888]!	; 0xb48

000104f0 <abort@plt>:
   104f0:	e28fc600 	add	ip, pc, #0, 12
   104f4:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   104f8:	e5bcfb40 	ldr	pc, [ip, #2880]!	; 0xb40

000104fc <close@plt>:
   104fc:	e28fc600 	add	ip, pc, #0, 12
   10500:	e28cca10 	add	ip, ip, #16, 20	; 0x10000
   10504:	e5bcfb38 	ldr	pc, [ip, #2872]!	; 0xb38

Disassembly of section .text:

00010508 <_start>:
   10508:	e3a0b000 	mov	fp, #0
   1050c:	e3a0e000 	mov	lr, #0
   10510:	e49d1004 	pop	{r1}		; (ldr r1, [sp], #4)
   10514:	e1a0200d 	mov	r2, sp
   10518:	e52d2004 	push	{r2}		; (str r2, [sp, #-4]!)
   1051c:	e52d0004 	push	{r0}		; (str r0, [sp, #-4]!)
   10520:	e59fc010 	ldr	ip, [pc, #16]	; 10538 <_start+0x30>
   10524:	e52dc004 	push	{ip}		; (str ip, [sp, #-4]!)
   10528:	e59f000c 	ldr	r0, [pc, #12]	; 1053c <_start+0x34>
   1052c:	e59f300c 	ldr	r3, [pc, #12]	; 10540 <_start+0x38>
   10530:	ebffffd6 	bl	10490 <__libc_start_main@plt>
   10534:	ebffffed 	bl	104f0 <abort@plt>
   10538:	0001094c 	.word	0x0001094c
   1053c:	000105f8 	.word	0x000105f8
   10540:	000108ec 	.word	0x000108ec

00010544 <call_weak_fn>:
   10544:	e59f3014 	ldr	r3, [pc, #20]	; 10560 <call_weak_fn+0x1c>
   10548:	e59f2014 	ldr	r2, [pc, #20]	; 10564 <call_weak_fn+0x20>
   1054c:	e08f3003 	add	r3, pc, r3
   10550:	e7932002 	ldr	r2, [r3, r2]
   10554:	e3520000 	cmp	r2, #0
   10558:	012fff1e 	bxeq	lr
   1055c:	eaffffd1 	b	104a8 <__gmon_start__@plt>
   10560:	00010aac 	.word	0x00010aac
   10564:	00000040 	.word	0x00000040

00010568 <deregister_tm_clones>:
   10568:	e59f0018 	ldr	r0, [pc, #24]	; 10588 <deregister_tm_clones+0x20>
   1056c:	e59f3018 	ldr	r3, [pc, #24]	; 1058c <deregister_tm_clones+0x24>
   10570:	e1530000 	cmp	r3, r0
   10574:	012fff1e 	bxeq	lr
   10578:	e59f3010 	ldr	r3, [pc, #16]	; 10590 <deregister_tm_clones+0x28>
   1057c:	e3530000 	cmp	r3, #0
   10580:	012fff1e 	bxeq	lr
   10584:	e12fff13 	bx	r3
   10588:	00021050 	.word	0x00021050
   1058c:	00021050 	.word	0x00021050
   10590:	00000000 	.word	0x00000000

00010594 <register_tm_clones>:
   10594:	e59f0024 	ldr	r0, [pc, #36]	; 105c0 <register_tm_clones+0x2c>
   10598:	e59f1024 	ldr	r1, [pc, #36]	; 105c4 <register_tm_clones+0x30>
   1059c:	e0411000 	sub	r1, r1, r0
   105a0:	e1a01141 	asr	r1, r1, #2
   105a4:	e0811fa1 	add	r1, r1, r1, lsr #31
   105a8:	e1b010c1 	asrs	r1, r1, #1
   105ac:	012fff1e 	bxeq	lr
   105b0:	e59f3010 	ldr	r3, [pc, #16]	; 105c8 <register_tm_clones+0x34>
   105b4:	e3530000 	cmp	r3, #0
   105b8:	012fff1e 	bxeq	lr
   105bc:	e12fff13 	bx	r3
   105c0:	00021050 	.word	0x00021050
   105c4:	00021050 	.word	0x00021050
   105c8:	00000000 	.word	0x00000000

000105cc <__do_global_dtors_aux>:
   105cc:	e92d4010 	push	{r4, lr}
   105d0:	e59f4018 	ldr	r4, [pc, #24]	; 105f0 <__do_global_dtors_aux+0x24>
   105d4:	e5d43000 	ldrb	r3, [r4]
   105d8:	e3530000 	cmp	r3, #0
   105dc:	18bd8010 	popne	{r4, pc}
   105e0:	ebffffe0 	bl	10568 <deregister_tm_clones>
   105e4:	e3a03001 	mov	r3, #1
   105e8:	e5c43000 	strb	r3, [r4]
   105ec:	e8bd8010 	pop	{r4, pc}
   105f0:	00021050 	.word	0x00021050

000105f4 <frame_dummy>:
   105f4:	eaffffe6 	b	10594 <register_tm_clones>

000105f8 <main>:
   105f8:	e92d4810 	push	{r4, fp, lr}
   105fc:	e28db008 	add	fp, sp, #8
   10600:	e24dd024 	sub	sp, sp, #36	; 0x24
   10604:	e50b0020 	str	r0, [fp, #-32]	; 0xffffffe0
   10608:	e50b1024 	str	r1, [fp, #-36]	; 0xffffffdc
   1060c:	e59f02a4 	ldr	r0, [pc, #676]	; 108b8 <main+0x2c0>
   10610:	ebffff9b 	bl	10484 <puts@plt>
   10614:	ebffff97 	bl	10478 <getuid@plt>
   10618:	e1a03000 	mov	r3, r0
   1061c:	e3530000 	cmp	r3, #0
   10620:	0a000003 	beq	10634 <main+0x3c>
   10624:	e59f0290 	ldr	r0, [pc, #656]	; 108bc <main+0x2c4>
   10628:	ebffff95 	bl	10484 <puts@plt>
   1062c:	e3e03000 	mvn	r3, #0
   10630:	ea00009d 	b	108ac <main+0x2b4>
   10634:	e59f1284 	ldr	r1, [pc, #644]	; 108c0 <main+0x2c8>
   10638:	e59f0284 	ldr	r0, [pc, #644]	; 108c4 <main+0x2cc>
   1063c:	ebffff9c 	bl	104b4 <open@plt>
   10640:	e50b0014 	str	r0, [fp, #-20]	; 0xffffffec
   10644:	e51b3014 	ldr	r3, [fp, #-20]	; 0xffffffec
   10648:	e3530000 	cmp	r3, #0
   1064c:	aa00000a 	bge	1067c <main+0x84>
   10650:	ebffffa0 	bl	104d8 <__errno_location@plt>
   10654:	e1a03000 	mov	r3, r0
   10658:	e5933000 	ldr	r3, [r3]
   1065c:	e1a00003 	mov	r0, r3
   10660:	ebffff8d 	bl	1049c <strerror@plt>
   10664:	e1a03000 	mov	r3, r0
   10668:	e1a01003 	mov	r1, r3
   1066c:	e59f0254 	ldr	r0, [pc, #596]	; 108c8 <main+0x2d0>
   10670:	ebffff7d 	bl	1046c <printf@plt>
   10674:	e3e0300f 	mvn	r3, #15
   10678:	ea00008b 	b	108ac <main+0x2b4>
   1067c:	ebffff92 	bl	104cc <getpagesize@plt>
   10680:	e1a03000 	mov	r3, r0
   10684:	e1a01003 	mov	r1, r3
   10688:	e59f323c 	ldr	r3, [pc, #572]	; 108cc <main+0x2d4>
   1068c:	e58d3004 	str	r3, [sp, #4]
   10690:	e51b3014 	ldr	r3, [fp, #-20]	; 0xffffffec
   10694:	e58d3000 	str	r3, [sp]
   10698:	e3a03001 	mov	r3, #1
   1069c:	e3a02003 	mov	r2, #3
   106a0:	e3a00000 	mov	r0, #0
   106a4:	ebffff85 	bl	104c0 <mmap@plt>
   106a8:	e1a02000 	mov	r2, r0
   106ac:	e59f321c 	ldr	r3, [pc, #540]	; 108d0 <main+0x2d8>
   106b0:	e5832000 	str	r2, [r3]
   106b4:	e59f3214 	ldr	r3, [pc, #532]	; 108d0 <main+0x2d8>
   106b8:	e5933000 	ldr	r3, [r3]
   106bc:	e3730001 	cmn	r3, #1
   106c0:	1a00000f 	bne	10704 <main+0x10c>
   106c4:	ebffff83 	bl	104d8 <__errno_location@plt>
   106c8:	e1a03000 	mov	r3, r0
   106cc:	e5933000 	ldr	r3, [r3]
   106d0:	e1a00003 	mov	r0, r3
   106d4:	ebffff70 	bl	1049c <strerror@plt>
   106d8:	e1a04000 	mov	r4, r0
   106dc:	ebffff7d 	bl	104d8 <__errno_location@plt>
   106e0:	e1a03000 	mov	r3, r0
   106e4:	e5932000 	ldr	r2, [r3]
   106e8:	e59f31e0 	ldr	r3, [pc, #480]	; 108d0 <main+0x2d8>
   106ec:	e5933000 	ldr	r3, [r3]
   106f0:	e1a01004 	mov	r1, r4
   106f4:	e59f01d8 	ldr	r0, [pc, #472]	; 108d4 <main+0x2dc>
   106f8:	ebffff5b 	bl	1046c <printf@plt>
   106fc:	e3e0300f 	mvn	r3, #15
   10700:	ea000069 	b	108ac <main+0x2b4>
   10704:	e51b3020 	ldr	r3, [fp, #-32]	; 0xffffffe0
   10708:	e3530002 	cmp	r3, #2
   1070c:	1a000015 	bne	10768 <main+0x170>
   10710:	e51b3024 	ldr	r3, [fp, #-36]	; 0xffffffdc
   10714:	e2833004 	add	r3, r3, #4
   10718:	e5933000 	ldr	r3, [r3]
   1071c:	e59f21b4 	ldr	r2, [pc, #436]	; 108d8 <main+0x2e0>
   10720:	e59f11b4 	ldr	r1, [pc, #436]	; 108dc <main+0x2e4>
   10724:	e1a00003 	mov	r0, r3
   10728:	ebffff6d 	bl	104e4 <__isoc99_sscanf@plt>
   1072c:	e1a03000 	mov	r3, r0
   10730:	e3530001 	cmp	r3, #1
   10734:	1a000005 	bne	10750 <main+0x158>
   10738:	e59f3198 	ldr	r3, [pc, #408]	; 108d8 <main+0x2e0>
   1073c:	e5933000 	ldr	r3, [r3]
   10740:	e1a01003 	mov	r1, r3
   10744:	e59f0194 	ldr	r0, [pc, #404]	; 108e0 <main+0x2e8>
   10748:	ebffff47 	bl	1046c <printf@plt>
   1074c:	ea000005 	b	10768 <main+0x170>
   10750:	e51b3024 	ldr	r3, [fp, #-36]	; 0xffffffdc
   10754:	e2833004 	add	r3, r3, #4
   10758:	e5933000 	ldr	r3, [r3]
   1075c:	e1a01003 	mov	r1, r3
   10760:	e59f017c 	ldr	r0, [pc, #380]	; 108e4 <main+0x2ec>
   10764:	ebffff40 	bl	1046c <printf@plt>
   10768:	e59f3160 	ldr	r3, [pc, #352]	; 108d0 <main+0x2d8>
   1076c:	e5933000 	ldr	r3, [r3]
   10770:	e2833004 	add	r3, r3, #4
   10774:	e5933000 	ldr	r3, [r3]
   10778:	e3c3260e 	bic	r2, r3, #14680064	; 0xe00000
   1077c:	e59f314c 	ldr	r3, [pc, #332]	; 108d0 <main+0x2d8>
   10780:	e5933000 	ldr	r3, [r3]
   10784:	e2833004 	add	r3, r3, #4
   10788:	e3822602 	orr	r2, r2, #2097152	; 0x200000
   1078c:	e5832000 	str	r2, [r3]
   10790:	e59f3138 	ldr	r3, [pc, #312]	; 108d0 <main+0x2d8>
   10794:	e5933000 	ldr	r3, [r3]
   10798:	e2833008 	add	r3, r3, #8
   1079c:	e5932000 	ldr	r2, [r3]
   107a0:	e59f3128 	ldr	r3, [pc, #296]	; 108d0 <main+0x2d8>
   107a4:	e5933000 	ldr	r3, [r3]
   107a8:	e2833008 	add	r3, r3, #8
   107ac:	e3c2260e 	bic	r2, r2, #14680064	; 0xe00000
   107b0:	e5832000 	str	r2, [r3]
   107b4:	e3a03000 	mov	r3, #0
   107b8:	e50b3010 	str	r3, [fp, #-16]
   107bc:	ea000022 	b	1084c <main+0x254>
   107c0:	e59f3108 	ldr	r3, [pc, #264]	; 108d0 <main+0x2d8>
   107c4:	e5933000 	ldr	r3, [r3]
   107c8:	e283301c 	add	r3, r3, #28
   107cc:	e3a02802 	mov	r2, #131072	; 0x20000
   107d0:	e5832000 	str	r2, [r3]
   107d4:	e3a03000 	mov	r3, #0
   107d8:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
   107dc:	ea000002 	b	107ec <main+0x1f4>
   107e0:	e51b3018 	ldr	r3, [fp, #-24]	; 0xffffffe8
   107e4:	e2833001 	add	r3, r3, #1
   107e8:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
   107ec:	e51b2018 	ldr	r2, [fp, #-24]	; 0xffffffe8
   107f0:	e59f30e0 	ldr	r3, [pc, #224]	; 108d8 <main+0x2e0>
   107f4:	e5933000 	ldr	r3, [r3]
   107f8:	e1520003 	cmp	r2, r3
   107fc:	bafffff7 	blt	107e0 <main+0x1e8>
   10800:	e59f30c8 	ldr	r3, [pc, #200]	; 108d0 <main+0x2d8>
   10804:	e5933000 	ldr	r3, [r3]
   10808:	e2833028 	add	r3, r3, #40	; 0x28
   1080c:	e3a02802 	mov	r2, #131072	; 0x20000
   10810:	e5832000 	str	r2, [r3]
   10814:	e3a03000 	mov	r3, #0
   10818:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
   1081c:	ea000002 	b	1082c <main+0x234>
   10820:	e51b3018 	ldr	r3, [fp, #-24]	; 0xffffffe8
   10824:	e2833001 	add	r3, r3, #1
   10828:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
   1082c:	e51b2018 	ldr	r2, [fp, #-24]	; 0xffffffe8
   10830:	e59f30a0 	ldr	r3, [pc, #160]	; 108d8 <main+0x2e0>
   10834:	e5933000 	ldr	r3, [r3]
   10838:	e1520003 	cmp	r2, r3
   1083c:	bafffff7 	blt	10820 <main+0x228>
   10840:	e51b3010 	ldr	r3, [fp, #-16]
   10844:	e2833001 	add	r3, r3, #1
   10848:	e50b3010 	str	r3, [fp, #-16]
   1084c:	e51b3010 	ldr	r3, [fp, #-16]
   10850:	e3530003 	cmp	r3, #3
   10854:	daffffd9 	ble	107c0 <main+0x1c8>
   10858:	e3a03000 	mov	r3, #0
   1085c:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
   10860:	ea000002 	b	10870 <main+0x278>
   10864:	e51b3018 	ldr	r3, [fp, #-24]	; 0xffffffe8
   10868:	e2833001 	add	r3, r3, #1
   1086c:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
   10870:	e51b3018 	ldr	r3, [fp, #-24]	; 0xffffffe8
   10874:	e3530ffa 	cmp	r3, #1000	; 0x3e8
   10878:	bafffff9 	blt	10864 <main+0x26c>
   1087c:	e59f304c 	ldr	r3, [pc, #76]	; 108d0 <main+0x2d8>
   10880:	e5933000 	ldr	r3, [r3]
   10884:	e2833034 	add	r3, r3, #52	; 0x34
   10888:	e5933000 	ldr	r3, [r3]
   1088c:	e2033302 	and	r3, r3, #134217728	; 0x8000000
   10890:	e3530000 	cmp	r3, #0
   10894:	0affffc6 	beq	107b4 <main+0x1bc>
   10898:	e59f0048 	ldr	r0, [pc, #72]	; 108e8 <main+0x2f0>
   1089c:	ebfffef8 	bl	10484 <puts@plt>
   108a0:	e51b0014 	ldr	r0, [fp, #-20]	; 0xffffffec
   108a4:	ebffff14 	bl	104fc <close@plt>
   108a8:	e3a03000 	mov	r3, #0
   108ac:	e1a00003 	mov	r0, r3
   108b0:	e24bd008 	sub	sp, fp, #8
   108b4:	e8bd8810 	pop	{r4, fp, pc}
   108b8:	0001095c 	.word	0x0001095c
   108bc:	0001098c 	.word	0x0001098c
   108c0:	00101002 	.word	0x00101002
   108c4:	000109b8 	.word	0x000109b8
   108c8:	000109c4 	.word	0x000109c4
   108cc:	fe200000 	.word	0xfe200000
   108d0:	00021054 	.word	0x00021054
   108d4:	000109e4 	.word	0x000109e4
   108d8:	0002104c 	.word	0x0002104c
   108dc:	00010a0c 	.word	0x00010a0c
   108e0:	00010a10 	.word	0x00010a10
   108e4:	00010a28 	.word	0x00010a28
   108e8:	00010a3c 	.word	0x00010a3c

000108ec <__libc_csu_init>:
   108ec:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
   108f0:	e1a07000 	mov	r7, r0
   108f4:	e59f6048 	ldr	r6, [pc, #72]	; 10944 <__libc_csu_init+0x58>
   108f8:	e59f5048 	ldr	r5, [pc, #72]	; 10948 <__libc_csu_init+0x5c>
   108fc:	e08f6006 	add	r6, pc, r6
   10900:	e08f5005 	add	r5, pc, r5
   10904:	e0466005 	sub	r6, r6, r5
   10908:	e1a08001 	mov	r8, r1
   1090c:	e1a09002 	mov	r9, r2
   10910:	ebfffecd 	bl	1044c <_init>
   10914:	e1b06146 	asrs	r6, r6, #2
   10918:	08bd87f0 	popeq	{r4, r5, r6, r7, r8, r9, sl, pc}
   1091c:	e3a04000 	mov	r4, #0
   10920:	e2844001 	add	r4, r4, #1
   10924:	e4953004 	ldr	r3, [r5], #4
   10928:	e1a02009 	mov	r2, r9
   1092c:	e1a01008 	mov	r1, r8
   10930:	e1a00007 	mov	r0, r7
   10934:	e12fff33 	blx	r3
   10938:	e1560004 	cmp	r6, r4
   1093c:	1afffff7 	bne	10920 <__libc_csu_init+0x34>
   10940:	e8bd87f0 	pop	{r4, r5, r6, r7, r8, r9, sl, pc}
   10944:	00010610 	.word	0x00010610
   10948:	00010608 	.word	0x00010608

0001094c <__libc_csu_fini>:
   1094c:	e12fff1e 	bx	lr

Disassembly of section .fini:

00010950 <_fini>:
   10950:	e92d4008 	push	{r3, lr}
   10954:	e8bd8008 	pop	{r3, pc}
