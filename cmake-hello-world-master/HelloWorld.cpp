#include <Speaker.h>
#include "gen/proto/cpp/helloworld/hello_world.pb.h"

using namespace std;
using namespace Hello;

int main(int argc, char *argv[]) {
  Speaker* speaker = new Speaker();

  speaker->sayHello();  

  helloworld::HelloRequest hello_request;
}
