$(eval $(call library,mldb_sample_plugin,plugin.cc,mldb))
$(LIB)/libmldb_sample_plugin.so: $(BIN)/mldb_runner

$(eval $(call mldb_unit_test,load_plugin_demo.py))
