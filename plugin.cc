/* sample_plugin.cc

   Jeremy Barnes, 11 September 2015
   Mich, 2016-12-14
   Copyright (c) 2015 Datacratic Inc.  All rights reserved.
 */

#include "mldb/types/value_description.h"
#include "mldb/core/function.h"
#include "mldb/core/plugin.h"
#include "mldb/rest/poly_entity.h"
#include "mldb/server/mldb_server.h"

using namespace MLDB;
using namespace std;

/// We are defining a new Function here, so we need to derive from
/// Datacratic::MLDB::Function and implement that interface.
struct HelloWorldFunction: public MLDB::Function {

    /// Our constructor knows about the MLDB server we're integrated
    /// into, its configuration and includes a progress object that
    /// we can optionally use to give feedback into initialization
    /// progress (we don't use it, as initialization is intstantaneous).
    HelloWorldFunction(MLDB::MldbEngine * server,
                       PolyConfig config,
                       std::function<bool (Json::Value)> onProgress)
        : MLDB::Function(server, config)
    {
    }

    /// Return the status of the plugin.  We return a simple constant
    /// string.  This is what will be returned from the
    /// GET /v1/functions/.../status API route.
    virtual Any getStatus() const
    {
        return std::string("A-OK");
    }

    /// Return the function information.  This tells MLDB about what the
    /// types of the input and output are, in order to allow for the
    /// function to be compiled as part of an SQL expression.  We
    /// return a single pin called "hello" with a string value, so that's
    /// what we return.
    virtual FunctionInfo getFunctionInfo() const
    {
        vector<KnownColumn> cols;
        cols.emplace_back(PathElement("world"),
                          make_shared<Utf8StringValueInfo>(),
                          COLUMN_IS_DENSE);

        FunctionInfo result;
        result.output = std::make_shared<RowValueInfo>(cols, SCHEMA_CLOSED);
        cols.clear();
        result.input.emplace_back(new RowValueInfo(cols, SCHEMA_CLOSED));
        return result;
    }

    /// Apply the function.  The applier contains information about the
    /// function itself, and the context gives us the input arguments.
    /// Since we don't do anything complicated or read the input, neither
    /// argument is used.
    virtual ExpressionValue apply(const FunctionApplier & applier,
                                  const ExpressionValue & context) const
    {
        // Create the string "world" as of the current timestamp.  In MLDB,
        // every data point has an associated timestamp at which it's known.
        ExpressionValue world("world", Date::now());

        return world;
    }
};

/// Create the plugin itself.  In our case, we only want the plugin as a
/// way to load up the HelloWorld function, so we don't add any
/// functionality over the base class.

struct SamplePlugin: public MLDB::Plugin {

    SamplePlugin(MldbEngine * server)
        : MLDB::Plugin(server)
    {
    }
    
    /// Override the getStatus method to return a custom method
    Any getStatus() const
    {
        return std::string("SamplePlugin is loaded");
    }
};

extern "C" {

/// This is the function that MLDB will call to initialize the plugin.
/// It needs to return a newly created plugin.
MLDB::Plugin * mldbPluginEnter(MldbEngine * server)
{
    return new SamplePlugin(server);
}

} // extern C

/// Put this declaration in "file scope" so it doesn't clash with other
/// plugin initializations.
namespace {

/// Register our function with MLDB.  We say here that our function
/// helloWorld takes an integer for configuration (which is ignored),
/// has the given description and has no documentation available.

Package package("mldb_sample_plugin");

static RegisterFunctionType<HelloWorldFunction, int>
regHelloWorldFunction(package,
                      "helloWorld",
                      "Sample function that always returns hello = \"world\"",
                      "No documentation available");
} // file scope
