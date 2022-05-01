// minirender.cpp: 定义应用程序的入口点。
//

#include "minirender.h"

using namespace std;
using namespace minirender;

static std::shared_ptr<Renderer> r;
int main(int argc, char** argv) {
  // ch0::DrawRedPoint();
  // ch1::FirstAttempt();
  // ch1::DrawWireframe("./resource/african_head/african_head.obj");
  // ch2::First();
  // ch2::Second();
  // ch2::Third("./resource/african_head/african_head.obj");
  // ch3::First("./resource/african_head/african_head.obj");
  // ch3::Second("./resource/african_head/african_head.obj");
  // ch4::First("./resource/african_head/african_head.obj");
  ch5::First("./resource/african_head/african_head.obj");
  ch5::Second("./resource/african_head/african_head.obj");
  ch5::Third("./resource/african_head/african_head.obj");
  cout << reinterpret_cast<int64_t>(&*r);
  return 0;
}