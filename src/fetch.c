#include "fetch.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include "hash.h"

static
size_t write_file(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return fwrite(ptr, size, nmemb, (FILE *)stream);
}

static
CURLcode download_package_source(const char *url, const char *dest_path)
{
    CURLcode res = CURLE_FAILED_INIT;
    CURL *curl = curl_easy_init();
    FILE *fp = NULL;

    if (curl == NULL)
        goto out;

    fp = fopen(dest_path, "wb");
    if (fp == NULL)
        goto out;

    fprintf(stderr, "preparing to fetch [%s]\n", url);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    res = curl_easy_perform(curl);
out:
    if (res != CURLE_OK)
        fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
    if (fp != NULL)
        fclose(fp);
    curl_easy_cleanup(curl);
    return res;
}


/**
 * fetch() - Download a package's source tarball and verify its hash.
 * @p: Package whose @src URL and @sha256 are used.
 * @dest_path: Destination path on disk.
 *
 * Shells out to curl(1) so scn does not link libcurl. The downloaded
 * file's sha256 is compared against @p->sha256.
 *
 * Return: FETCH_OK_VAL on success, a tagged error otherwise.
 */
fetch_error fetch(const pkg *p, const char *dest_path) {
    // TODO: replace this with execvpe or libcurl
    CURLcode status = download_package_source(p->src, dest_path);

    if (status != CURLE_OK) {
        return (fetch_error){
            .kind = FETCH_E_IO,
            .url = p->src,
            .errno_val = (int)status,
        };
    }

    char actual[65] = {0};
    if (!sha256_verify_file(dest_path, p->sha256, actual)) {
        fprintf(stderr, "hash mismatch: actual=[%s], expected=[%s]\n", actual, p->sha256);

        return (fetch_error){
            .kind = FETCH_E_HASH_MISMATCH,
            .url = p->src,
            .expected_sha = p->sha256,
            .actual_sha = strdup(actual),
        };
    }

    return FETCH_OK_VAL;
}
