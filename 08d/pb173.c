#include <linux/io.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/pci_ids.h>
#include <linux/interrupt.h>

static irqreturn_t my_handler(int irq, void* data, struct pt_regs* regs)
{
//    return my_device_raised_interrupt ? IRQ_HADNDLED : IRQ_NONE;
    return IRQ_HANDLED;
}

static int my_probe(struct pci_dev *pdev, const struct pci_device_id *id) 
{
    printk(KERN_INFO "probe: bus->number: %2.x driver_data: %lu pci_slot: %x pci_func: %x\n", pdev->bus->number, id->driver_data, 
	PCI_SLOT(pdev->devfn), PCI_FUNC(pdev->devfn));

    int en = pci_enable_device(pdev);
    if (en < 0) {
	return en;
    }
    int req_reg = pci_request_region(pdev, 0, "res0");
    if (req_reg < 0) {
	return req_reg;
    }
    void* mem = pci_ioremap_bar(pdev, 0);
    pci_set_drvdata(pdev, mem);
    
    u32 bridge_id = readl(mem);
    u32 bridge_build = readl(mem + 0x0004);
    
    printk(KERN_INFO "id: %4x build %4x\n", bridge_id, bridge_build);
    
    int rok   = 2000 + ((bridge_build & 0xf0000000) >> 28);
    int mesic =        (bridge_build & 0x0f000000) >> 24;
    int den   =        (bridge_build & 0x00ff0000) >> 16;
    int hodina =       (bridge_build & 0x0000ff00) >> 8;
    int minuta =       (bridge_build & 0x000000ff) >> 0;
    printk(KERN_INFO "vyrobeno: rok: %i mesic: %i den: %i hodina: %i minuta %i\n", rok, mesic, den, hodina, minuta);
    printk(KERN_INFO "vyrobeno: %i.%i.%i %i:%i\n", den, mesic, rok, hodina, minuta);
    
//    void* my_data = kmalloc(10);
    int ret = request_irq(pdev->irq, my_handler, IRQF_SHARED, "my", mem);

    return 0;
}

static void my_remove(struct pci_dev *pdev) 
{
    printk(KERN_INFO "probe: bus->number: %2.x pci_slot: %x pci_func: %x\n", pdev->bus->number, 
	PCI_SLOT(pdev->devfn), PCI_FUNC(pdev->devfn));

    void* mem = pci_get_drvdata(pdev);
	
    free_irq(pdev->irq, mem);

    iounmap(mem);
    pci_release_region(pdev, 0);
    pci_disable_device(pdev);
	
}

static struct  pci_device_id my_table[] = {
    { PCI_DEVICE(0x18ec, 0xc058) },
    { 0, }
};

MODULE_DEVICE_TABLE(pci, my_table);

static struct pci_driver my_pci_driver = {
    .name = "my_driver",
    .id_table = my_table,
    .probe = my_probe,
    .remove = my_remove,
};

static int my_init(void)
{
	printk(KERN_DEBUG "my_init\n");
	return pci_register_driver(&my_pci_driver);
}

static void my_exit(void)
{
	printk(KERN_DEBUG "my_exit\n");

	pci_unregister_driver(&my_pci_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
