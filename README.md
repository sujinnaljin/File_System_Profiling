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

![ext4](https://github.com/sujinnaljin/File_System_Profiling/blob/master/Data%20%26%20Preprocessing%20%26%20Plotting/ext4_plotting.png)

**LILFS2**

![nilfs2](https://github.com/sujinnaljin/File_System_Profiling/blob/master/Data%20%26%20Preprocessing%20%26%20Plotting/nilfs2_plotting.png)

## 실행 방법
1. 원래 커널 소스에 있는 linux/block/blk-core.c와 linux/fs/nilfs2/segbuf.c 우리가 작성한 blk-core.c와 segbuf.c파일로 대체 (modify로 수정 부분 검색 가능)
2. 커널 컴파일 후 해당 커널로 부팅
```
uname -r //현재 커널 버전 확인
vi /boot/grub/grub.cfg //여기서 menu_entry, sub_entry 중에 원하는 커널 버전 확인. menu_entry는 0, sub_entry는 1>0, 1>1, .. 1>n
vi /etc/default/grub //GRUB-DEFUALT 값 확인 후 원하는 값으로 변경 ex GRUB-DEFUALT = "1>3"
sudo update-grub
reboot
uname -r //부팅된 커널 버전 확인
```
3. Linux Kernel Module 작성 → basic.c 에 해당
4. Makefile 작성. 이때 KDIR는 커널 소스 경로.
5. make 명령어 실행 
6. 커널 모듈 올리기 → sudo insmod basic.ko
7. 수행하고자 하는 fs가 mount 된 dir 로 이동
8.  iozone -a -i 0 명령어를 통해 write 수행
9.  cd /proc/myproc_dir/ 통해 경로 이동
10. sudo bash -c 'echo  문자 > myproc_file' 명령어 수행
11. dmesg를 통해 출력 결과 확인

## 파일 설명

**/KernelCode/blk-core.c**

경로: 커널 소스파일 디렉토리/block/blk-core.c 설명:

- line 36 - 시간을 받아오기 위해 <linux/time.h> 헤더를 포함시켰다.
- line 53 - queue를 구현했다. 그리고 queue를 export해서 모듈에서 사용할 수 있게 하였다
- line 2214 - submit_bio 함수 안 time, sector번호, system이름을 받아 queue에 저장하였다.

**/KernelCode/segbuf.c**

경로: 커널 소스파일 디렉토리/fs/nilfs2/segbuf.c 설명: Nilfs2에서 submit_bio를 호출하는 함수는 nilfs_segbuf_submit_bio()로 segbuf.c 파일에 정의되어있다. 이 nilfs_segbuf_submit_bio() 함수에서 bi_bdev내의 super_block 구조체인 bd_super에 인자로 받은 segbuf (nilfs_segment_buffer 구조체)에 들어있는 super_bd(super_block구조체)를 넣어주었다.

**/LKM/Makefile**

경로: ~/모듈 소스 저장 디렉토리 설명: KDIR에 커널 소스 경로를 저장하여 mybasic.c를 컴파일 하는데 쓰인다.

**/LKM/mybasic.c**

경로: ~/모듈 소스 저장 디렉토리 설명: 모듈을 구현한 파일이다. 안에 custom_write이라는 함수를 구현하여 원하는 정보를 로그에 출력하게 하였다.

**/LKM/mybasic.ko**

경로: ~/모듈 소스 저장 디렉토리 설명: mybasic.c 와 Makefile로 컴파일한 모듈 파일

**/Data & Preprocessing & Plotting/iozone_log_ext4.txt**

ext4 디스크에서 수행했을 때 iozone 수행했을때 출력되는 로그 파일.

**/Data & Preprocessing & Plotting/iozone_log_ext4.txt**

iozone_log_ext4**.**txt에서 변환된 csv  파일. 시간에 대한 데이터의 표현에 한계가 있다.

**/Data & Preprocessing & Plotting/iozone_log_ext4&nilfs2.txt**

nilfs2 디스크에서 수행했을 때 iozone 수행했을때 출력되는 로그 파일. ext4와 혼재되어 있다.

**/Data & Preprocessing & Plotting/iozone_log_ext4&nilfs2.csv**

iozone_log_ext4**.**txt에서 변환된 csv  파일. 시간에 대한 데이터의 표현에 한계가 있다.

**/Data & Preprocessing & Plotting/iozone_log_edited_nilfs2.txt**

iozone_log_ext4&nilfs2.txt 파일에서 nilfs2만을 뽑아낸 로그 파일.

**/Data & Preprocessing & Plotting/iozone_log_edited_nilfs2.csv**

iozone_log_ext4&nilfs2.txt t에서 변환된 csv  파일. 시간에 대한 데이터의 표현에 한계가 있다.

**/Data & Preprocessing & Plotting/log_preprocessing.ipynb**

로그에 nilfs2와 ext4가 혼재되어 있을때, nilfs2에 해당하는 로그만 빼내기 위해서 preprocessing 하는 파일. iozone_log_ext4&nilfs2.txt 파일을 로드해 해당 결과를 iozone_log_edited_nilfs2.txt로 추출한다.

**/Data & Preprocessing & Plotting/plotting_fs.ipynb**

로그 파일 기반으로한 data를 plotting 하기 위한 파일.

**/Data & Preprocessing & Plotting/ext4_plotting.png**

ext4 로그에 대해 plotting 한 결과 파일.

**/Data & Preprocessing & Plotting/nilfs2_plotting.png.png**

nilfs2 로그에 대해 plotting 한 결과 파일.
