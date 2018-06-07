#include "CopyElision.h"
#include "IREquality.h"
#include "IRMutator.h"
#include "IROperator.h"
#include "Substitute.h"

namespace Halide {
namespace Internal {

using std::map;
using std::pair;
using std::string;
using std::vector;

namespace {

struct CopyPair {
    string prod; // Copy from
    string cons; // Store into
};

/** If function 'f' operation only involves pointwise copy from another
  * function, return the name of the function from which it copies from.
  * If the function being copied from is a tuple, we have to ensure that 'f'
  * copies the whole tuple and not only some of the tuple values; otherwise,
  * treat it as non pointwise copies. For non pointwise copy or if 'f' has
  * update definitions or is an extern function, return an empty string.
  */
string get_pointwise_copy_producer(const Function &f) {
    if (f.has_update_definition() || f.has_extern_definition()) {
        return "";
    }
    string prod;
    for (const Expr &val : f.values()) {
        if (const Call *call = val.as<Call>()) {
            if (call->call_type == Call::Halide) {
                // Check if it is a pointwise copy
            }
        }
    }
    return prod;
}

/** Return all pairs of functions which operation only involves pointwise copy
  * of another function and the function from which it copies from. Ignore
  * functions that have updates or are extern functions. */
vector<CopyPair> get_pointwise_copies(const vector<string> &order,
                                      const map<string, Function> &env) {
    vector<CopyPair> pointwise_copies;
    for (const string &f : order) {
        string copied_from = get_pointwise_copy_producer(env.at(f));
        if (!copied_from.empty()) {
            pointwise_copies.push_back({copied_from, f});
        }
    }
    return pointwise_copies;
}

/** Find unnecessary copies that can be eliminated. */
class FindUnnecessaryCopies : public IRVisitor {
public:
    vector<pair<string, string>> unnecessary_copies;
private:
    using IRVisitor::visit;

    DeviceAPI current_device_api = DeviceAPI::None;

    bool is_simple_copy(const Provide *op) {
        return false;
    }

    void visit(const For *op) {

    }

    void visit(const Call *op) {
        if (op->call_type == Call::Halide || op->call_type == Call::Image) {
        }
        for (size_t i = 0; i < op->args.size(); i++) {
            op->args[i].accept(this);
        }
        IRVisitor::visit(op);
    }

    void visit(const Provide *op) {

    }
};

class CopyElision : public IRMutator2 {
public:
    CopyElision(const map<string, Function> &env) : env(env) {}
private:
    const map<string, Function> &env;

    using IRMutator2::visit;

    /*Expr visit(const Call *op) override {
        if (op->call_type == Call::Halide || op->call_type == Call::Image) {
            // Multi-dimensional load

        }
    }

    Stmt visit(const Provide *op) override {
        // Multi-dimensional store
    }*/
};

} // anonymous namespace

Stmt copy_elision(Stmt s,
                  const vector<string> &order,
                  const map<string, Function> &env) {
    s = CopyElision(env).mutate(s);
    return s;
}

}  // namespace Internal
}  // namespace Halide
