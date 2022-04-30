// minirender.cpp: 定义应用程序的入口点。
//

#include "minirender.h"

using namespace std;
using namespace minirender;

int main(int argc, char** argv) {
  // ch0::DrawRedPoint();
  ch1::FirstAttempt();
  ch1::DrawWireframe("./resource/african_head/african_head.obj");
  ch2::First();
  ch2::Second();
  ch2::Third("./resource/african_head/african_head.obj");
  ch3::First("./resource/african_head/african_head.obj");
  ch3::Second("./resource/african_head/african_head.obj");
  ch4::First("./resource/african_head/african_head.obj");
  ch5::First("./resource/african_head/african_head.obj");
  ch5::Second("./resource/african_head/african_head.obj");
  return 0;
}