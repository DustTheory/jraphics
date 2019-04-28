#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>

const unsigned int W = 500, H = 500;


// Takes in a complex number to map it to a point on the coordinate system
// We're mapping W*H area to rectangle area on coordinate system: (-2, -2), (-2, 2), (2, -2), (2, 2)

struct Color{
	int r, g, b, a;
};

double scale_ratio_x = 4.0/W; // just default
double scale_ratio_y = 4.0/H; // just default
std::complex<double> bottom_left(-2,-2);
std::complex<double> top_right(2, 2);

void set_camera(std::complex<double>bl, std::complex<double>tr){
	bottom_left = bl;
	top_right = tr;
	scale_ratio_x = (top_right.real() - bottom_left.real())/W;
	scale_ratio_y = (top_right.imag() - bottom_left.imag())/H;
	std::cout << scale_ratio_x << std::endl;
}

inline std::complex<double> pixel_to_complex(int x){
	x /= 4;
	int a = x % W;
	int b = x / W;
	return std::complex<double>(scale_ratio_x*a-top_right.real(), scale_ratio_y*b-top_right.imag());
}

inline std::complex<double> pixel_to_complex(int x, int y){
	return std::complex<double>(scale_ratio_x*x-top_right.real(), scale_ratio_y*y-top_right.imag());
}

#define MAX_ITER_MBS 100
#define EPS_MBS 1e-300
const long double EPS_SQRT_MBS = sqrtl(EPS_MBS);
const double iter_color_ratio = MAX_ITER_MBS/255;

inline Color get_color(double d){
	double red = (fabs(0.25-d)/0.75)*255;
	double green = (fabs(0.5-d)/0.5)*255;
	double blue = (fabs(0.75-d)/0.75)*255;
	return {(int)red, (int)green, (int)blue, 255};
}

inline Color mandelbrot_point(std::complex<double> c){

		std::complex<double> p(0,0);
		int i;
		bool converges = false;
		p = p*p + c;
		for(i = 1; i < MAX_ITER_MBS; i++){
			p = p*p + c;
			if(p.real() > 10000000 || p.imag() > 10000000){
					break;
			}
			long double dist = p.real()*p.real() + p.imag()*p.imag();
			if(dist < EPS_SQRT_MBS){
				converges = true;
				break;
			}
			if(i == MAX_ITER_MBS-1){
				converges = true;
				break;
			}
		}
		Color color = converges ? Color{0, 0, 0, 255} : get_color((double)i/MAX_ITER_MBS);
		return color;
}

void zoom_in(int x, int y, double zoom){
	std::complex<double> p = pixel_to_complex(x, y);
	double cw = (top_right.real() - bottom_left.real())/(2*zoom);
	double ch = (top_right.imag() - bottom_left.imag())/(2*zoom);
	set_camera(
		{-1*p.real()-cw, -1*p.imag()-ch},
		{-1*p.real()+cw, -1*p.imag()+ch}
	);
}

const int texture_size = W*H*4;
const int half_texture_size = texture_size/2;
void render_set_first(sf::Uint8* pixels){

	for(register int i = 0; i < texture_size; i+=4){
		std::complex<double> cx = pixel_to_complex(i);
		Color c{0,0,0,0};
		if(cx.imag() > 0){
			int a = (i/4) % W;
			int b = (i/4) / W;
			int pos = ((H-b)*W+a)*4;
			c = {pixels[pos],
				 pixels[pos+1],
				 pixels[pos+2],
				 pixels[pos+3]
				 };
		}else
			c = mandelbrot_point(cx);
		pixels[i] = c.r;   // r
		pixels[i+1] = c.g; // g
		pixels[i+2] = c.b; // b
		pixels[i+3] = c.a; // a
	}
}

void render_set(sf::Uint8* pixels){
	Color c;
	for(register int i = 0; i < texture_size; i+=4){
		c = mandelbrot_point(pixel_to_complex(i));
		pixels[i] = c.r;   // r
		pixels[i+1] = c.g; // g
		pixels[i+2] = c.b; // b
		pixels[i+3] = c.a; // a
	}
}

int main()
{

	sf::RenderWindow window(sf::VideoMode(W, H), "Mandelbrot set"); 

	sf::Uint8* pixels = new sf::Uint8[W*H*4];

	sf::RenderTexture renderTexture;
	renderTexture.create(W, H, true);

	sf::Texture texture;
	texture.create(W, H);
	sf::Sprite sprite(texture);

	set_camera({-2, -2}, {2, 2});
	
	render_set_first(pixels);
	texture.update(pixels);
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Left){
					zoom_in(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 20);
					render_set(pixels);
					texture.update(pixels);
				}
			}
			
        }

        window.clear();
		window.draw(sprite);
        window.display();
    }
    return 0;
}
