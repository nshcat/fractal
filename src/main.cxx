#include <ut/console_color.hxx>

int main()
{
	::std::cout << ut::foreground(ut::console_color::bright_magenta) << "Hello World!" << ut::reset_color << ::std::endl;
}
