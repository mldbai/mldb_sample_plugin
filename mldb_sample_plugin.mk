$(eval $(call library,mldb_sample_plugin,plugin.cc,mldb))

$(eval $(call mldb_unit_test,load_plugin_demo.py))
