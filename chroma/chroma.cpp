
#include <iostream>
#include <string>

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>

namespace gil = boost::gil;

#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include "image.hpp"
#include "matrix.hpp"


using namespace std;

int main(int argc, char const *argv[]) {

  try
  {
    po::options_description desc("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
                                 "<<       Chroma keying app.       <<\n"
                                 "<< Author: Alberto Ramos Sánchez. <<\n"
                                 "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n"
                                 "Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("fg", po::value<string>()->required(), "foreground image file (PNG)")
      ("bg", po::value<string>()->required(), "background image file (PNG)")
      ("o", po::value<string>()->default_value("../output.png"), "output image file (PNG)")
      ("key-color", po::value< vector<int> >()->multitoken()->required(), "key color (RGB)")
      ("t", po::value<double>()->default_value(1), "threshold")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if(vm.count("help")) { cout << desc << endl; return 0; }

    po::notify(vm);


    gil::rgb8_pixel_t key_color;
    vector<int> key_color_vec;
    if(!vm["key-color"].empty() && (key_color_vec = vm["key-color"].as<vector<int>>()).size() == 3 ) {
      int red   = key_color_vec[0];
      int green = key_color_vec[1];
      int blue  = key_color_vec[2];

      if((red   > 255   || red < 0) ||
         (green > 255 || green < 0) ||
         (blue  > 255  || blue < 0) )
        { cerr << "[ERROR] Color values must be in range 0 - 255" << endl; return -1; }

      key_color = gil::rgb8_pixel_t{(unsigned char)red, (unsigned char)green, (unsigned char)blue};
    } else {
      cerr << "[ERROR] Expected 3 colour channels" << endl;
      return 1;
    }

    double threshold = vm["t"].as<double>();
    if(threshold < 0) cerr << "[ERROR] Threshold must be greater than 0" << endl;

    string fg_file = vm["fg"].as<string>();
    gil::rgb8_image_t fg_image;
    image_lib::read_image(fg_image, fg_file);

    string bg_file = vm["bg"].as<string>();
    gil::rgb8_image_t bg_image;
    image_lib::read_image(bg_image, bg_file);

    gil::rgb8_image_t res(fg_image.width(), fg_image.height());

    // resample background
    gil::rgb8_image_t bg_resampled_image(fg_image.width(), fg_image.height());
    gil::resize_view(gil::const_view(bg_image), gil::view(bg_resampled_image), gil::bilinear_sampler());

    image_lib::chroma_keying(res,
                             fg_image,
                             bg_resampled_image,
                             key_color,
                             threshold);

    string out = vm["o"].as<string>();
    image_lib::write_image(res, out);

  } catch(exception& e) {
    cerr << "[ERROR] " << e.what() << endl;
  } catch(...) {
    cerr << "Unknown exception!" << endl;
  }

  return 0;
}
