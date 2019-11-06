#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <asm/pgtable_types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/fdtable.h>
#include <linux/slab.h>
#include <linux/unistd.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("790_792");
MODULE_VERSION("1.0.1");
/*MY sys_call_table address*/
//ffffffff82000260
void **system_call_table_addr;

asmlinkage long (*temp_open) (const char*, int, mode_t);
asmlinkage ssize_t (*temp_write) (int, const void*, size_t);

asmlinkage long hook_open(const char* filename, int flags, mode_t mode)
{
    char buffer[1024];
    copy_from_user(buffer, filename, 1024);
    printk(KERN_INFO "Process name opens file: %s", current->comm);
    printk(KERN_INFO "Hooked open: filename = %s\n", buffer);
    return temp_open(filename, flags, mode);
}
asmlinkage long hook_write(int fd, const void* buf, size_t len)
{   
    char buffer[1024];
    char* filename = d_path(&fcheck_files(current->files, fd)->f_path, buffer, 1024);
    printk(KERN_INFO "Process name writes file: %s", current->comm);
    printk(KERN_INFO "Hooked write: fd = %s, len = %d\n", filename, (int)len);
    return temp_write(fd, buf, len);
}
int make_rw(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW)
    {
        pte->pte |=_PAGE_RW;
    }
    return 0;
}
int make_ro(unsigned long address)
{
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte & ~_PAGE_RW;
    return 0;
}
static int __init entry_point(void)
{
    printk(KERN_INFO "Hook loaded successfully..\n");
    /*MY sys_call_table address*/
    system_call_table_addr = (void*)0xffffffff82000260;
    // system_call_table_addr = (void **) kallsyms_lookup_name("sys_call_table");
    /* Replace custom syscall with the correct system call name (write,open,etc) to hook*/
    temp_open = system_call_table_addr[__NR_open];
    temp_write = system_call_table_addr[__NR_write];
    /*Disable page protection*/
    make_rw((unsigned long)system_call_table_addr);
    /*Change syscall to our syscall function*/
    system_call_table_addr[__NR_open] = hook_open;
    system_call_table_addr[__NR_write] = hook_write;
    return 0;
}
static void __exit exit_point(void)
{
    printk(KERN_INFO "Unloaded Hook successfully\n");
    /*Disable page protection*/
    system_call_table_addr[__NR_open] = temp_open;
    system_call_table_addr[__NR_write] = temp_write;
    /*Renable page protection*/
    make_ro((unsigned long)system_call_table_addr);
}
module_init(entry_point);
module_exit(exit_point);
