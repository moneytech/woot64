#include <cpu.hpp>
#include <debug.hpp>
#include <pci.hpp>
#include <string.hpp>

#define PCI_SHOW_LIST 1

Vector<PCI::Device *> PCI::Devices(32, 16);

PCI::Device::Device(PCI::Address address, uint16_t vid, uint16_t did, uint8_t cls, uint8_t subCls, uint8_t progif) :
    ::Device(::Device::Root),
    Address(address),
    VendorID(vid),
    DeviceID(did),
    Class(cls),
    SubClass(subCls),
    ProgIF(progif)
{
}

void PCI::Check()
{
    DEBUG("Enumerating PCI devices...\n");
    Config config;
    ReadConfigData(&config, 0);
    if((config.HeaderType & 0x80) == 0)
        CheckBus(0);
    else
    {
        for(uint8_t func = 0; func < 8; func++)
        {
            if(ReadConfigDWord(0x80000000 | (func << 8)) == 0xFFFF)
                break;
            CheckBus(func);
        }
    }
    DEBUG("PCI bus scan complete.\n");
}

void PCI::CheckBus(uint8_t bus)
{
    for(uint8_t device = 0; device < 32; device++)
        CheckDevice(bus, device);
}

void PCI::CheckDevice(int8_t bus, uint8_t device)
{
    uint32_t vid = ReadConfigDWord(0x80000000 | (bus << 16) | (device << 11)) & 0xFFFF;
    for(uint8_t func = 0; func < 8 && vid != 0xFFFF; func++)
        CheckFunction(bus, device, func);
}

void PCI::CheckFunction(uint8_t bus, uint8_t device, uint8_t func)
{
    Config config;
    uint32_t address = 0x80000000 | (bus << 16) | (device << 11) | (func << 8);
    ReadConfigData(&config, address);
    if(config.VendorID == 0xFFFF || config.VendorID == 0x0000)
        return;

#if(PCI_SHOW_LIST)
    DEBUG("  %d.%d.%d class: %02X subclass: %02X if: %02X id: %04X:%04X\n",
          bus, device, func,
          (uint32_t)config.Class,
          *(uint8_t *)&config.SubClass,
          config.ProgIF,
          config.VendorID,
          config.DeviceID);
#endif // PCI_LIST

    Devices.Append(new Device(address, config.VendorID, config.DeviceID, config.Class, config.SubClass, config.ProgIF));

    if(config.Class == 0x06 && config.SubClass == 0x09)
        CheckBus(config.Header.PCI2PCI.SecondaryBusNumber);
}

void PCI::Initialize()
{
    Check();
}

uint8_t PCI::ReadConfigByte(PCI::Address address)
{
    bool cs = cpuDisableInterrupts();
    _outl(0x0CF8, address);
    uint8_t r = _inb(0x0CFC);
    cpuRestoreInterrupts(cs);
    return r;
}

void PCI::WriteConfigByte(PCI::Address address, uint8_t value)
{
    bool cs = cpuDisableInterrupts();
    _outl(0x0CF8, address);
    _outb(0x0CFC, value);
    cpuRestoreInterrupts(cs);
}

uint16_t PCI::ReadConfigWord(PCI::Address address)
{
    bool cs = cpuDisableInterrupts();
    _outl(0x0CF8, address);
    uint16_t r = _inw(0x0CFC);
    cpuRestoreInterrupts(cs);
    return r;
}

void PCI::WriteConfigWord(PCI::Address address, uint16_t value)
{
    bool cs = cpuDisableInterrupts();
    _outl(0x0CF8, address);
    _outw(0x0CFC, value);
    cpuRestoreInterrupts(cs);
}

uint32_t PCI::ReadConfigDWord(PCI::Address address)
{
    bool cs = cpuDisableInterrupts();
    _outl(0x0CF8, address);
    uint32_t r = _inl(0x0CFC);
    cpuRestoreInterrupts(cs);
    return r;
}

void PCI::WriteConfigDWord(PCI::Address address, uint32_t value)
{
    bool cs = cpuDisableInterrupts();
    _outl(0x0CF8, address);
    _outl(0x0CFC, value);
    cpuRestoreInterrupts(cs);
}

void PCI::ReadConfigData(PCI::Config *config, PCI::Address address)
{
    uint32_t *configPtr = (uint32_t *)config;
    for(uint32_t i = 0; i < 4; i++)
    {
        address = (address & 0xFFFFFF00) | (i << 2);
        configPtr[i] = ReadConfigDWord(address);
    }

    uint32_t dwords = config->HeaderType == 0x02 ? 18 : 16; // CardBus bridge header is a bit larger
    for(uint32_t i = 4; i < dwords; i++)
    {
        address = (address & 0xFFFFFF00) | (i << 2);
        configPtr[i] = ReadConfigDWord(address);
    }
}

void PCI::WriteConfigData(PCI::Address address, PCI::Config *config)
{
    uint32_t *configPtr = (uint32_t *)config;
    for(uint32_t i = 0; i < 4; i++)
    {
        address = (address & 0xFFFFFF00) | (i << 2);
        WriteConfigDWord(address, configPtr[i]);
    }

    uint32_t dwords = config->HeaderType == 0x02 ? 18 : 16; // CardBus bridge header is a bit larger
    for(uint32_t i = 4; i < dwords; i++)
    {
        address = (address & 0xFFFFFF00) | (i << 2);
        WriteConfigDWord(address, configPtr[i]);
    }
}

void PCI::Cleanup()
{
    for(Device *dev : Devices)
        delete dev;
    Devices.Clear();
}
