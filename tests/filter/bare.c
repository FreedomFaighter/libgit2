#include "clar_libgit2.h"
#include "crlf.h"

static git_repository *g_repo = NULL;
static git_blob_filter_options filter_opts = GIT_BLOB_FILTER_OPTIONS_INIT;

void test_filter_bare__initialize(void)
{
	cl_fixture_sandbox("crlf.git");
	cl_git_pass(git_repository_open(&g_repo, "crlf.git"));

	filter_opts.flags |= GIT_BLOB_FILTER_NO_SYSTEM_ATTRIBUTES;
	filter_opts.flags |= GIT_BLOB_FILTER_ATTTRIBUTES_FROM_HEAD;
}

void test_filter_bare__cleanup(void)
{
	git_repository_free(g_repo);
	cl_fixture_cleanup("crlf.git");
}

void test_filter_bare__all_crlf(void)
{
	git_blob *blob;
	git_buf buf = { 0 };

	cl_git_pass(git_revparse_single(
		(git_object **)&blob, g_repo, "a9a2e89")); /* all-crlf */

	cl_assert_equal_s(ALL_CRLF_TEXT_RAW, git_blob_rawcontent(blob));

	cl_git_pass(git_blob_filter(&buf, blob, "file.bin", &filter_opts));

	cl_assert_equal_s(ALL_CRLF_TEXT_RAW, buf.ptr);

	cl_git_pass(git_blob_filter(&buf, blob, "file.crlf", &filter_opts));

	/* in this case, raw content has crlf in it already */
	cl_assert_equal_s(ALL_CRLF_TEXT_AS_CRLF, buf.ptr);

	cl_git_pass(git_blob_filter(&buf, blob, "file.lf", &filter_opts));

	/* we never convert CRLF -> LF on platforms that have LF */
	cl_assert_equal_s(ALL_CRLF_TEXT_AS_CRLF, buf.ptr);

	cl_git_pass(git_blob_filter(&buf, blob, "file.txt", &filter_opts));

	/* in this case, raw content has crlf in it already */
	cl_assert_equal_s(ALL_CRLF_TEXT_AS_CRLF, buf.ptr);

	git_buf_dispose(&buf);
	git_blob_free(blob);
}

void test_filter_bare__from_lf(void)
{
	git_blob *blob;
	git_buf buf = { 0 };

	cl_git_pass(git_revparse_single(
		(git_object **)&blob, g_repo, "799770d")); /* all-lf */

	cl_assert_equal_s(ALL_LF_TEXT_RAW, git_blob_rawcontent(blob));

	cl_git_pass(git_blob_filter(&buf, blob, "file.bin", &filter_opts));

	cl_assert_equal_s(ALL_LF_TEXT_RAW, buf.ptr);

	cl_git_pass(git_blob_filter(&buf, blob, "file.crlf", &filter_opts));

	/* in this case, raw content has crlf in it already */
	cl_assert_equal_s(ALL_LF_TEXT_AS_CRLF, buf.ptr);

	cl_git_pass(git_blob_filter(&buf, blob, "file.lf", &filter_opts));

	/* we never convert CRLF -> LF on platforms that have LF */
	cl_assert_equal_s(ALL_LF_TEXT_AS_LF, buf.ptr);

	git_buf_dispose(&buf);
	git_blob_free(blob);
}

void test_filter_bare__sanitizes(void)
{
	git_blob *blob;
	git_buf buf = GIT_BUF_INIT;

	cl_git_pass(git_revparse_single(
		(git_object **)&blob, g_repo, "e69de29")); /* zero-byte */

	cl_assert_equal_i(0, git_blob_rawsize(blob));
	cl_assert_equal_s("", git_blob_rawcontent(blob));

	cl_git_pass(git_blob_filter(&buf, blob, "file.bin", &filter_opts));
	cl_assert_equal_sz(0, buf.size);
	cl_assert_equal_s("", buf.ptr);
	git_buf_dispose(&buf);

	cl_git_pass(git_blob_filter(&buf, blob, "file.crlf", &filter_opts));
	cl_assert_equal_sz(0, buf.size);
	cl_assert_equal_s("", buf.ptr);
	git_buf_dispose(&buf);

	cl_git_pass(git_blob_filter(&buf, blob, "file.lf", &filter_opts));
	cl_assert_equal_sz(0, buf.size);
	cl_assert_equal_s("", buf.ptr);
	git_buf_dispose(&buf);

	git_blob_free(blob);
}

