#include <stdint.h>
#include <stdlib.h>

#include "pci/register.h"
#include "pci/pci.h"
#include "pci/io.h"
#include "kernel/kernel.h"

static void scan_device(struct pci_device_location *location)
{
    uint32_t id = PCI_MAKE_ID(location->bus, location->dev, location->func);
    struct pci_device device;

    device.vendor_id = pci_inb_16(id, PCI_CONFIG_VENDOR_ID);
    if (device.vendor_id == 0xffff)
        return;
    device.device_id = pci_inb_16(id, PCI_CONFIG_DEVICE_ID);
    device.class_code = pci_inb_8(id, PCI_CONFIG_CLASS_CODE);
    device.subclass = pci_inb_8(id, PCI_CONFIG_SUBCLASS);
    device.prog_intf = pci_inb_8(id, PCI_CONFIG_PROG_INTF);

    struct device_driver *driver = lookup_driver(device.vendor_id, device.device_id);

#ifdef DEBUG
    char *name;
    if (driver != NULL) {
        name = driver->name;
    } else {
        name = "unknown device";
    }
    printk("  %02x:%02x:%d: 0x%04x/0x%04x (%s)\n", 
            location->bus, location->dev, location->func, 
            device.vendor_id, device.device_id, name);
#endif

    if (driver != NULL && driver->init_func != NULL)
        driver->init_func(location);
}

static void scan_pci_bus(void)
{
#ifdef DEBUG
    printk("Scanning PCI bus for devices..\n");
#endif
    struct pci_device_location loc;
    for (uint32_t bus = 0; bus < 256; bus++)
        for (uint32_t dev = 0; dev < 32; dev++)
            for (uint32_t func = 0; func < 8; func++) {
                loc.bus = bus;
                loc.dev = dev;
                loc.func = func;
                scan_device(&loc);
            }
}

void init_pci(void)
{
    scan_pci_bus();
}
