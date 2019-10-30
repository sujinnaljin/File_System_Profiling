# File_System_Profiling

## 주제		
LFS(Log-structured File System) & Ext4 File System Profiling

## 환경		
가상머신 : Oracle VM VirtualBox

운영체제 : Ubuntu 16.04 LTS 

커널 : linux-4.4.0

파일시스템 : Ext4, Nilfs2

## 목적
1.	Log Structured 파일 시스템인 Nilfs2와 리눅스 시스템에서 널리 사용되는 Ext4 파일 시스템의 쓰기 동작 방식을 알아보고 비교 분석한다.
2.	다양한 파일 시스템을 지원하기 위해 Virtual File System에서 사용되는 구조체에 대해 알아본다.
3.	Proc file system을 이용하여 실험 중 데이터를 수집한다.

## 요구사항
1.	쓰기 동작 패턴을 확인하기 위한 코드를 작성한다.

A.	디스크에 쓰기가 발생하는 순간, 해당 block number 및 시간 값을 메모리에 저장하도록 작성한다. (block_no, time)

B.	버퍼가 꽉 파게 되면 버퍼의 시작 지점부터 다시 저장할 수 있도록 한다.

C.	커널의 block layer 코드를 수정하여 기능을 구현한다.

D.	버퍼에 저장된 내용을 출력하기 위한 proc 인터페이스를 작성한다.

2.	VFS의 구조체를 사용하여 쓰기 파일을 확인하기 위한 코드를 작성한다.

A.	VFS에서 사용하는 구조체를 사용하여 쓰기 파일이 사용하는 파일 시스템의 이름을 확인하는 코드를 작성한다.

B.	파일 시스템의 이름을 출력하기 위한 proc 인터페이스를 작성한다.

3.	테스트용 벤치마크 툴을 사용하여 쓰기 블록 프로파일링을 수행한다.

A.	iozone benchmark tool을 사용하여 테스트를 수행한다.

B.	Nilfs2 파티션 및 Ext4 파티션 상에서 테스트를 수행한다.

C.	테스트 결과를 그래프로 작성하여 결과값을 제출한다. 

(x축 : 측정 시간, y축 : block number)

## 설명
본 과제는 Log Structured 파일 시스템과 Ext4 사용시 실제 디스크에 데이터가 쓰여지는 방식이 어떻게 다른지 파악해 보는 것을 목표로 한다.

Nilfs2는 리눅스 상에서 구현된 Log Structured 파일 시스템이며 Ext4는 Ordinary 파일 시스템으로서, 이를 이용하여 Nilfs2와 Ext4 파일 시스템의 쓰기 특성을 비교할 수 있도록 한다.

디스크에 쓰기가 발생하는 순간을 측정하는 코드를 작성해 봄으로서, 커널의 디스크 쓰기 루틴, 특히 block layer를 코드 레벨로 이해하는 데에 도움이 될 것이다.

또한, Virtual File System에서 쓰기 파일에 대한 파일 시스템 명을 출력함으로써 VFS에서 사용되는 구조체에 대한 이해를 하는 데에 도움이 될 것이다.

## 결과
**EXT4**
![ext4](https://github.com/sujinnaljin/File_System_Profiling/blob/master/Result/ext4_plotting.png)

**LILFS2**
![nilfs2](https://github.com/sujinnaljin/File_System_Profiling/blob/master/Result/nilfs2_plotting.png)

## 실행 방법
1. 원래 커널 소스에 있는 linux/block/blk-core.c와 linux/fs/nilfs2/segbuf.c 우리가 작성한 blk-core.c와 segbuf.c파일로 대체 (modify로 수정 부분 검색 가능)
2. 커널 컴파일 후 해당 커널로 부팅
3. Linux Kernel Module 작성 → basic.c 에 해당
4. Makefile 작성. 이때 KDIR는 커널 소스 경로.
5. make 명령어 실행 
6. 커널 모듈 올리기 → sudo insmod basic.ko
7. 수행하고자 하는 fs가 mount 된 dir 로 이동
8.  iozone -a -i 0 명령어를 통해 write 수행
9.  cd /proc/myproc_dir/ 통해 경로 이동
10. sudo bash -c 'echo  문자 > myproc_file' 명령어 수행
11. dmesg를 통해 출력 결과 확인
