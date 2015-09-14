all:
	$(MAKE) -C native/HBV
	$(MAKE) -C native/LRGV
	$(MAKE) -C native/WDS
	$(MAKE) -C native/Radar
	$(MAKE) -C native/LakeProblem