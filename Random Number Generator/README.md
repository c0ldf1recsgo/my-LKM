LINUX KERNEL MODULE
Linux Kernel Module Create a Random Number
- Chương trình tạo ra module dùng để tạo ra một số ngẫu nhiên
- Module này sẽ tạo ra một character device để cho phép các tiến trình ở user space có thể open và read các số ngẫu nhiên.

# File 1:
## number.c 
File này là chương trình ở Kernel Space dùng để sinh ra một số ngẫu nhiên, cho phép các tiến trình ở user space có thể open và read các số ngẫu nhiên

File bao gồm:
### static int __init _init_numGenerator(void)
Hàm này dùng để khởi tạo khi xây dựng một driver. Hàm được gọi khi module được load lên.  

### static void __exit _exit_numGenerator(void)
Hàm này sẽ dọn những gì được làm trước khi gỡ module. Hàm được gọi trong quá trình gỡ module. 

### static int device_Open(struct inode *inodep, struct file *filep)
Hàm được gọi khi nhận yêu cầu mở từ user space.

### static ssize_t device_Read(struct file *filep, char* buffer, size_t len, loff_t *offset)
Hàm được gọi khi user space muốn truy cập dữ liệu từ device.

### static int device_Close(struct inode *inodep, struct file *filep)
Hàm được gọi khi đóng device.

# File 2:
## generator.c
Đây là chương trình thuôc user space. 
Chương trình này sẽ gọi lệnh **open("/dev/numGenerator", O_RDWR)** để yêu cầu mở device được tạo từ number.c trên để tạo ra một số ngẫu nhiên. 
- Sau khi mở thành công, đọc số ngẫu nhiên từ device gửi đến. 
- Sau khi đọc thì xuất ra số ngẫu nhiên đó. 


# Hướng dẫn sử dụng
1. Mở Terminal trong thư mục chứa 2 file trên
2. Nhập lệnh "make" để build chương trình 
3. Cài module bằng lệnh:
```sudo insmod number.ko```
4. Chạy file test bằng lệnh:
```sudo ./a.out```
5. Gỡ bỏ module bằng lệnh: 
```sudo rmmod number```

