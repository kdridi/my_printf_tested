all				:
				$(MAKE) -C lib/my $@

clean			:
				$(MAKE) -C lib/my $@
				$(MAKE) -C tests $@

fclean			:
				$(MAKE) -C lib/my $@
				$(MAKE) -C tests $@

re				:	fclean all

tests_run		:
				$(MAKE) -C tests all
				gcovr --exclude tests