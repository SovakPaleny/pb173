#include <linux/io.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/pci_ids.h>

static int my_init(void)
{

	struct pci_dev *pdev = NULL;
	while((pdev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, pdev))) {
		printk(KERN_INFO "%2.x:%.2x.%.2x vendor: %.2x device: %.2x", pdev->bus->number, PCI_SLOT(pdev->devfn), PCI_FUNC(pdev->devfn), 
		pdev->vendor, pdev->device);
	}

	return -EIO;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
