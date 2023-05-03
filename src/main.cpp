
#include "defines.hpp"
#include "img_generator.hpp"


int main( int argc, char ** argv )
{
	try
	{
		RT_ISICG::ImgGenerator gen(600, 400);
		gen.setup( RT_ISICG::ImgGenerator::Projet );
		gen.generate("image.jpg");
		return EXIT_SUCCESS;
	}
	catch ( const std::exception & e )
	{
		std::cerr << "Exception caught:" << std::endl << e.what() << std::endl;
	}
}
