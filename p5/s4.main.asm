000013f0 <main>:
    13f0:	8d 4c 24 04          	lea    0x4(%esp),%ecx
    13f4:	83 e4 f0             	and    $0xfffffff0,%esp
    13f7:	ff 71 fc             	push   -0x4(%ecx)
    13fa:	55                   	push   %ebp
    13fb:	89 e5                	mov    %esp,%ebp
    13fd:	57                   	push   %edi
    13fe:	56                   	push   %esi
    13ff:	53                   	push   %ebx
    1400:	51                   	push   %ecx
    1401:	81 ec 28 02 00 00    	sub    $0x228,%esp
    1407:	e8 34 fd ff ff       	call   1140 <__x86.get_pc_thunk.bx>
    140c:	81 c3 a0 2b 00 00    	add    $0x2ba0,%ebx
    1412:	8b 51 04             	mov    0x4(%ecx),%edx
    1415:	65 a1 14 00 00 00    	mov    %gs:0x14,%eax
    141b:	89 45 e4             	mov    %eax,-0x1c(%ebp)
    141e:	31 c0                	xor    %eax,%eax
    1420:	8b 83 48 00 00 00    	mov    0x48(%ebx),%eax
    1426:	8b 38                	mov    (%eax),%edi
    1428:	83 39 02             	cmpl   $0x2,(%ecx)
    142b:	74 6a                	je     1497 <main+0xa7>
    142d:	83 ec 0c             	sub    $0xc,%esp
    1430:	ff b3 64 00 00 00    	push   0x64(%ebx)
    1436:	e8 8a fe ff ff       	call   12c5 <insert1>
    143b:	83 c4 04             	add    $0x4,%esp
    143e:	ff b3 68 00 00 00    	push   0x68(%ebx)
    1444:	e8 cf fe ff ff       	call   1318 <insert2>
    1449:	83 c4 04             	add    $0x4,%esp
    144c:	ff b3 5c 00 00 00    	push   0x5c(%ebx)
    1452:	e8 6e fe ff ff       	call   12c5 <insert1>
    1457:	83 c4 04             	add    $0x4,%esp
    145a:	ff b3 6c 00 00 00    	push   0x6c(%ebx)
    1460:	e8 60 fe ff ff       	call   12c5 <insert1>
    1465:	83 c4 04             	add    $0x4,%esp
    1468:	ff b3 68 00 00 00    	push   0x68(%ebx)
    146e:	e8 52 fe ff ff       	call   12c5 <insert1>
    1473:	83 c4 04             	add    $0x4,%esp
    1476:	ff b3 60 00 00 00    	push   0x60(%ebx)
    147c:	e8 44 fe ff ff       	call   12c5 <insert1>
    1481:	83 c4 10             	add    $0x10,%esp
    1484:	be 01 00 00 00       	mov    $0x1,%esi
    1489:	8b 83 48 00 00 00    	mov    0x48(%ebx),%eax
    148f:	89 85 d0 fd ff ff    	mov    %eax,-0x230(%ebp)
    1495:	eb 35                	jmp    14cc <main+0xdc>
    1497:	83 ec 08             	sub    $0x8,%esp
    149a:	8d 83 0b e1 ff ff    	lea    -0x1ef5(%ebx),%eax
    14a0:	50                   	push   %eax
    14a1:	ff 72 04             	push   0x4(%edx)
    14a4:	e8 17 fc ff ff       	call   10c0 <fopen@plt>
    14a9:	89 c7                	mov    %eax,%edi
    14ab:	83 c4 10             	add    $0x10,%esp
    14ae:	e9 7a ff ff ff       	jmp    142d <main+0x3d>
    14b3:	83 ec 04             	sub    $0x4,%esp
    14b6:	56                   	push   %esi
    14b7:	ff b5 d4 fd ff ff    	push   -0x22c(%ebp)
    14bd:	6a 01                	push   $0x1
    14bf:	e8 0c fc ff ff       	call   10d0 <__printf_chk@plt>
    14c4:	83 c4 10             	add    $0x10,%esp
    14c7:	eb 1e                	jmp    14e7 <main+0xf7>
    14c9:	83 c6 01             	add    $0x1,%esi
    14cc:	8d 83 0d e1 ff ff    	lea    -0x1ef3(%ebx),%eax
    14d2:	89 85 d4 fd ff ff    	mov    %eax,-0x22c(%ebp)
    14d8:	83 fe 06             	cmp    $0x6,%esi
    14db:	74 5b                	je     1538 <main+0x148>
    14dd:	8b 85 d0 fd ff ff    	mov    -0x230(%ebp),%eax
    14e3:	39 38                	cmp    %edi,(%eax)
    14e5:	74 cc                	je     14b3 <main+0xc3>
    14e7:	83 ec 04             	sub    $0x4,%esp
    14ea:	57                   	push   %edi
    14eb:	68 00 02 00 00       	push   $0x200
    14f0:	8d 85 e4 fd ff ff    	lea    -0x21c(%ebp),%eax
    14f6:	50                   	push   %eax
    14f7:	e8 64 fb ff ff       	call   1060 <fgets@plt>
    14fc:	83 c4 10             	add    $0x10,%esp
    14ff:	85 c0                	test   %eax,%eax
    1501:	74 d5                	je     14d8 <main+0xe8>
    1503:	83 ec 04             	sub    $0x4,%esp
    1506:	6a 0a                	push   $0xa
    1508:	6a 00                	push   $0x0
    150a:	8d 85 e4 fd ff ff    	lea    -0x21c(%ebp),%eax
    1510:	50                   	push   %eax
    1511:	e8 ca fb ff ff       	call   10e0 <strtol@plt>
    1516:	89 85 d4 fd ff ff    	mov    %eax,-0x22c(%ebp)
    151c:	e8 67 fe ff ff       	call   1388 <pop>
    1521:	89 c2                	mov    %eax,%edx
    1523:	8b 85 d4 fd ff ff    	mov    -0x22c(%ebp),%eax
    1529:	83 c0 02             	add    $0x2,%eax
    152c:	83 c4 10             	add    $0x10,%esp
    152f:	39 c2                	cmp    %eax,%edx
    1531:	74 96                	je     14c9 <main+0xd9>
    1533:	e8 05 fd ff ff       	call   123d <fail>
