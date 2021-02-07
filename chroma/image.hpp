
// image.hpp
// author: Alberto Ramos Sánchez <alberto.ramos104@alu.ulpgc.es>
// creation date: january 2021
// Description: This is the header file of image_lib functions of the chroma keying application


#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <boost/gil/extension/io/png.hpp>
namespace gil = boost::gil;
#include "matrix.hpp"


namespace image_lib {

  // io

  void read_image(gil::rgb8_image_t& img, string& filename);

  void write_image(gil::rgb8_image_t& img, string& filename);

  // image masks

  void mask_image(gil::rgb8_image_t& image, mat_lib::matrix<double>& mask);

  void add_image(gil::rgb8_image_t& res, gil::rgb8_image_t& im1, gil::rgb8_image_t& im2);

  // chroma

  void rgb2hsv(double& hue, double& saturation, double& value,
               gil::rgb8_pixel_t& px);

  void rgb2hsv(mat_lib::matrix<double>& hue,
               mat_lib::matrix<double>& saturation,
               mat_lib::matrix<double>& value,
               gil::rgb8_image_t& image);

  void hsv_distance(mat_lib::matrix<double>& distance,
                    mat_lib::matrix<double>& hue,
                    mat_lib::matrix<double>& saturation,
                    mat_lib::matrix<double>& value,
                    double hue_key,
                    double sat_key,
                    double val_key
                  );

  void chroma_keying(gil::rgb8_image_t& result,
                     gil::rgb8_image_t fg_image,
                     gil::rgb8_image_t bg_image,
                     gil::rgb8_pixel_t& key_color,
                     double threshold);

}

#endif
