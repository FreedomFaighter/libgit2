static void test_patch(
	git_buf_dispose(&actual);
	git_str contents = GIT_STR_INIT;
	git_str_dispose(&contents);
	git_str actual = GIT_STR_INIT;
	git_str_dispose(&actual);
	git_str *buf = (git_str *)payload;
		git_str_put(buf, hunk->header, hunk->header_len);
		git_str_put(buf, line->content, line->content_len);
	return git_str_oom(buf) ? -1 : 0;
	git_str actual = GIT_STR_INIT;
	git_str_dispose(&actual);
	git_str old_binary_base85;
	git_str new_binary_base85;
	git_str_encode_base85(&diff_data->old_binary_base85,
	git_str_encode_base85(&diff_data->new_binary_base85,
	git_str_dispose(&diff_data.old_binary_base85);
	git_str_dispose(&diff_data.new_binary_base85);