#ifndef HALIDE_COPY_ELISION
#define HALIDE_COPY_ELISION

#include <map>

#include "IR.h"

/** \file
 * Defines a lowering pass that eliminates unnecessary copies.
 */

namespace Halide {
namespace Internal {

/** Find unnecessary copies and remove them from the Stmt. This doesn't touch
 * Store/Load nodes and so must be called before storage_flattening.
 */
Stmt copy_elision(Stmt s,
                  const std::vector<std::string> &order,
                  const std::map<std::string, Function> &env);

}  // namespace Internal
}  // namespace Halide

#endif
