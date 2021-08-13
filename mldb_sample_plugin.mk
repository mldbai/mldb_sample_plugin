$(eval $(call library,mldb_sample_plugin,plugin.cc,mldb $(MLDB_PLUGIN_AUTO_LIBS)))
$(LIB)/libmldb_sample_plugin$(SO_EXTENSION): $(BIN)/mldb_runner

$(eval $(call mldb_unit_test,load_plugin_demo.py))
$(TESTS)/load_plugin_demo.py.passed:	$(LIB)/libmldb_sample_plugin$(SO_EXTENSION)
