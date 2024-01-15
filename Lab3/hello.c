#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("IO-16 Oleh Buhaienko");
MODULE_DESCRIPTION("Laboratorna robota 3");
MODULE_LICENSE("GPL");

static uint total = 1;

module_param(total, uint, 0444); 
MODULE_PARM_DESC(total, "Count of entries to process"); 

struct custom_data {
  struct list_head node;
  ktime_t time_stamp;
};

static LIST_HEAD(custom_list_head);

static int __init custom_init_function(void) 
{
  int counter;
  
  if (total == 0) { 
    printk(KERN_WARNING "total is zero\n"); 
  } else if (total > 10) { 
    printk(KERN_WARNING "total more 10, setting to max limit\n"); 
    return -EINVAL;
  }

  for (counter = 0; counter < total; counter++) { 
    struct custom_data *item = kmalloc(sizeof(struct custom_data), GFP_KERNEL);
    item->time_stamp = ktime_get();
    list_add_tail(&item->node, &custom_list_head);
    printk(KERN_INFO "Entry number: %d\n", counter);
  }

  return 0;
}

static void __exit custom_exit_function(void) 
{
  struct custom_data *entry, *temp;
  int index = 0; 

  list_for_each_entry_safe(entry, temp, &custom_list_head, node) {
    printk(KERN_INFO "Entry %d; Timestamp = %lld\n", index++, ktime_to_ns(entry->time_stamp));
    list_del(&entry->node);
    kfree(entry);
  }
  printk(KERN_INFO "Module removed successfully\n");
}

module_init(custom_init_function);
module_exit(custom_exit_function);

