#include <functional>

namespace tt3d {
namespace Math {

typedef std::function<double(double)> Integrable;
typedef std::function<double (Integrable, double, double)> Integrator;

}
}
