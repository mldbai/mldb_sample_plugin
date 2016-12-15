#
# load_plugin_demo.py
# Mich, 2015-12-22
# This file is part of MLDB. Copyright 2015 Datacratic. All rights reserved.
#

mldb = mldb_wrapper.wrap(mldb)  # noqa

hello_world_url = '/v1/types/functions/helloWorld'

# the plugin doesn't exist
try:
    mldb.get(hello_world_url)
except mldb_wrapper.ResponseException as exc:  # noqa
    assert exc.response.status_code == 404
else:
    assert False, "should not be here"
#assert res['statusCode'] == 404, str(res)

# have MLDB load it
mldb.put('/v1/plugins/sample', {
    'type' : 'sharedLibrary',
    'params' : {
        'address' : 'build/x86_64/lib/',
        'library' : 'libmldb_sample_plugin.so',
        'apiVersion' : '1.0.0',
        'allowInsecureLoading' : True
    }
})

# now it exists
mldb.log(mldb.get(hello_world_url))

# let's make use of it
mldb.put('/v1/functions/hello', {
    'type' : 'helloWorld'
})
mldb.log(mldb.query("SELECT hello({})"))

mldb.script.set_return("success")
