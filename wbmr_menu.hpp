#include "main_menu.hpp"
#include "wbmr_device.hpp"

class WBMR_MainMenu : public MainMenu
{
public:
    WBMR_MainMenu(WBMR_Device &dev);

protected:
    int onItemSelected(unsigned int n) override;

private:
    void setItemValues();

    static const MenuItem choices[];
    WBMR_Device &device;
};

class WBMR_InputModeMenu : public Menu
{
public:
    WBMR_InputModeMenu(Position pos, WBMR_Device &dev);
    
protected:
    int onItemSelected(unsigned int n) override;

private:
    void setItemValues();

    static const MenuItem choices[];
    WBMR_Device &device;
    std::vector<MenuItem> items;
    unsigned int input_map[7];
    unsigned int input_count;
};
