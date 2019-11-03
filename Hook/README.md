Chương trình hook một System Call
* Ghi chú:
    - Tiến trình bị kill khi thực hiện cài module

File duy nhất hook.c
Các hàm có trong file 
## asmlinkage long hook_open(const char* filename, int flags, umode_t mode)
## asmlinkage long hook_write(unsigned int fd, const char* buf, size_t len)
Dùng để hook

## int make_rw(unsigned long address)
Xóa lớp bảo vệ của syscall

## int make_ro(unsigned long address)
Mở lại lớp bảo vệ của syscall

## static int __init entry_point(void)
## static void __init exit_point(void)
Hai hàm được gọi khi chạy lệnh insmod và rmmod

Hướng dẫn sử dụng:
1. Tìm địa chỉ Syscall Table:
``` cat /boot/System.map-XXX | grep sys_call_table ```
XXX là phiên bản kernel đang máy đang sử dụng <br />
Ví dụ 
``` cat /boot/System.map-5.3.7-050307-generic | grep sys_call_table ```
2. Copy địa chỉ syscall table nhậm được vào file hook.c tại dòng 56
``` system_call_table_addr = (void*)0xYYY; ``` <br />
YYY là địa chỉ syscall table <br />
Ví dụ
``` system_call_table_addr = (void*)0xffffffff82000260; ```
3. Mở Terminal trong folder chứa file hook.c
4. Build hook:
``` make ```
5. Insert module:
``` sudo insmod hook.ko ```
6. Remove module:
``` sudo rmmod hook ```
