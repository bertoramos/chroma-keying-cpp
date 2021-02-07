
#include <boost/gil/extension/io/png.hpp>
#include <string>

#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <system_error>
#include <cmath>

#include "matrix.hpp"
#include "image.hpp"

namespace gil = boost::gil;
using namespace std;

#define EPS 1e-16

void image_lib::read_image(gil::rgb8_image_t& img, string& filename) { gil::read_and_convert_image(filename, img, gil::png_tag() ); }

void image_lib::write_image(gil::rgb8_image_t& img, string& filename) { gil::write_view(filename, gil::view(img), gil::png_tag()); }


void image_lib::mask_image(gil::rgb8_image_t& image, mat_lib::matrix<double>& mask)
{
  if (image.width() != mask.columns() || image.height() != mask.rows()) {
    ostringstream str_stream;
    str_stream << "size mismatch! cannot mask image ("
      << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

    throw invalid_argument(str_stream.str());
  }

  gil::rgb8_view_t v = gil::view(image);
  auto b = v.begin();

  for (size_t h = 0; h < image.height(); h++) {
    for (size_t w = 0; w < image.width(); w++) {

      double red   = (double) (*b)[0];
      double green = (double) (*b)[1];
      double blue  = (double) (*b)[2];

      unsigned char new_red   = round(red * mask[h][w]);
      unsigned char new_green = round(green * mask[h][w]);
      unsigned char new_blue  = round(blue * mask[h][w]);

      *b = gil::rgb8_pixel_t{new_red, new_green, new_blue};

      b++;
    }
  }
}

void image_lib::add_image(gil::rgb8_image_t& res, gil::rgb8_image_t& im1, gil::rgb8_image_t& im2)
{

  if (im1.width() != im2.width() || im1.height() != im2.height()) {
    ostringstream str_stream;
    str_stream << "size mismatch! cannot add images ("
      << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

    throw invalid_argument(str_stream.str());
  }

  gil::rgb8_view_t vw_im1 = gil::view(im1);
  auto iter_im1 = vw_im1.begin();

  gil::rgb8_view_t vw_im2 = gil::view(im2);
  auto iter_im2 = vw_im2.begin();

  gil::rgb8_view_t vw_res = gil::view(res);
  auto iter_res = vw_res.begin();

  for (size_t h = 0; h < im1.height(); h++) {
    for (size_t w = 0; w < im2.width(); w++) {

      int r1 = (int) (*iter_im1)[0];
      int g1 = (int) (*iter_im1)[1];
      int b1 = (int) (*iter_im1)[2];

      int r2 = (int) (*iter_im2)[0];
      int g2 = (int) (*iter_im2)[1];
      int b2 = (int) (*iter_im2)[2];

      unsigned char new_red   = min(r1 + r2, 255);
      unsigned char new_green = min(g1 + g2, 255);
      unsigned char new_blue  = min(b1 + b2, 255);

      *iter_res = gil::rgb8_pixel_t{new_red, new_green, new_blue};

      iter_im1++;
      iter_im2++;
      iter_res++;
    }
  }
}


void image_lib::rgb2hsv(double& hue, double& saturation, double& value,
             gil::rgb8_pixel_t& px)
{
  double R = px[0]/255.0, G = px[1]/255.0, B = px[2]/255.0;

  double v = max(max(R, G), B);
  double x = min(min(R, G), B);

  double s = (v - x)/v;

  if(abs(s) < EPS) {
    hue = 0.0;
    saturation = s;
    value = v;
    return;
  }

  double r = (v - R)/(v - x);
  double g = (v - G)/(v - x);
  double b = (v - B)/(v - x);

  double h = 0.0;
  if(abs(R - v) < EPS) {
    h = abs(G-x) < EPS ? 5+b : 1-g;
  }
  if(abs(G - v) < EPS) {
    h = abs(B-x) < EPS ? 1+r : 3-b;
  }
  if(abs(B - v) < EPS) {
    h = abs(R-x) < EPS ? 3+g : 5-r;
  }

  hue = h/6.0;
  saturation = s;
  value = v;

}

void image_lib::rgb2hsv(mat_lib::matrix<double>& hue,
             mat_lib::matrix<double>& saturation,
             mat_lib::matrix<double>& value,
             gil::rgb8_image_t& image)
{

  mat_lib::matrix<double> ht(image.height(), image.width());
  hue = ht;

  mat_lib::matrix<double> st(image.height(), image.width());
  saturation = st;

  mat_lib::matrix<double> vt(image.height(), image.width());
  value = vt;

  gil::rgb8_view_t vw = gil::view(image);
  auto iter = vw.begin();

  for (size_t h = 0; h < image.height(); h++) {
    for (size_t w = 0; w < image.width(); w++) {

      double hval, sval, vval;

      rgb2hsv(hval, sval, vval, *iter);

      hue[h][w] = hval;
      saturation[h][w] = sval;
      value[h][w] = vval;

      iter++;
    }
  }

}

void image_lib::hsv_distance(mat_lib::matrix<double>& distance,
                  mat_lib::matrix<double>& hue,
                  mat_lib::matrix<double>& saturation,
                  mat_lib::matrix<double>& value,
                  double hue_key,
                  double sat_key,
                  double val_key
                 )
{

  // distancia hue
  mat_lib::matrix<double> diff_hue = (hue - hue_key).abs();
  mat_lib::matrix<double> dist_hue = diff_hue.min(diff_hue * -1.0 + 1.0);

  // distancia saturacion
  mat_lib::matrix<double> dist_sat = (saturation - sat_key).abs();

  // distancia total
  distance = ((dist_hue^2.0) + (dist_sat^2.0)) / pow(0.5, 2.0) + 1.0;

}

void image_lib::chroma_keying(gil::rgb8_image_t& result,
                   gil::rgb8_image_t fg_image,
                   gil::rgb8_image_t bg_image,
                   gil::rgb8_pixel_t& key_color,
                   double threshold)
{
  if (fg_image.width() != bg_image.width() || fg_image.height() != bg_image.height())
  {
    ostringstream str_stream;
    str_stream << "size mismatch! cannot apply chroma keying ("
      << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

    throw invalid_argument(str_stream.str());
  }

  // create hue / saturation matrices
  mat_lib::matrix<double> hmat;
  mat_lib::matrix<double> smat;
  mat_lib::matrix<double> vmat;
  image_lib::rgb2hsv(hmat, smat, vmat, fg_image);

  // convert key color to hsv
  double hkey, skey, vkey;
  rgb2hsv(hkey, skey, vkey, key_color);

  // create distance matrix
  mat_lib::matrix<double> dist;
  image_lib::hsv_distance(dist, hmat, smat, vmat, hkey, skey, vkey);

  // create masks
  mat_lib::matrix<double> fg_mask = (dist > threshold);

  mat_lib::matrix<double> bg_mask = (dist < threshold); // 1 - fg_mask

  mask_image(fg_image, fg_mask);
  mask_image(bg_image, bg_mask);

  add_image(result, fg_image, bg_image);

}
