#include <newt.h>
#include <string>

class ErrorPopup
{
public:
    ErrorPopup(int y, int x, std::string body);

    void Display();
    
private:
    int begin_x;
    int begin_y;
    int width;
    int height;
    const std::string message;
};
