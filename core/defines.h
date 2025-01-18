/**
 * @file
 * @brief A file containing generic defines
 */
#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>

//! Default permissions for directories
#define DIRPERMS                                                               \
    (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
//! Max threads used in thread pool.
#define MAX_CONNECTIONS 5
//! Maximum number of files allowed in the hierarchy.
#define MAX_FILES (1 << 10)

#define PORT_SIZE 5

#define DEL 127

#define ERROR -1

#define SUCCESS 0

//! Maximum method length in HTTP/1.1
#define METHODLEN 8
//! Length of "HTTP/x.x"
#define VERSIONLEN 9
//! Status code length in HTTP/x.x
#define STATUSLEN 3
//! HTTP version
#define HTTP_VERSION "HTTP/1.1"
//! Buffer size used for HTTP content types.
#define CONTENT_TYPE_LEN (1 << 6)
//! Buffer size used for an entire HTTP message.
#define MESSAGE_SIZE (1 << 20)
//! Buffer size used for an HTTP header.
#define HEADER_SIZE (1 << 16)
//! Maximum allowed URI (and Hostname) length.
#define URI_PATH_MAX (1 << 10)
//! Buffer size used for content and use data.
#define BUFFER_SIZE (1 << 19)
//! Data directory
#define ROOT "data"
// 1xx (informational), 204 (no content) and 304 (not modified) MUST NOT include
// a message-body.
//! X-Macro definition for HTTP status codes
#define HTTP_STATUS_MAP(X)                                                     \
    X(200, OK, OK)                                                             \
    X(201, CREATED, Created)                                                   \
    X(202, ACCEPTED, Accepted)                                                 \
    X(204, NO_CONTENT, No Content)                                             \
    X(205, RESET_CONTENT, Reset Content)                                       \
    X(400, BAD_REQUEST, Bad Request)                                           \
    X(403, FORBIDDEN, Forbidden)                                               \
    X(404, NOT_FOUND, Not Found)                                               \
    X(405, METHOD_NOT_ALLOWED, Method Not Allowed)                             \
    X(411, LENGTH_REQUIRED, Length Required)                                   \
    X(413, CONTENT_TOO_LARGE, Content Too Large)                               \
    X(414, URI_TOO_LONG, URI Too Long)                                         \
    X(418, IM_A_TEAPOT, Im a teapot)                                           \
    X(500, INTERNAL_SERVER_ERROR, Internal Server Error)                       \
    X(501, NOT_IMPLEMENTED, Not Implemented)                                   \
    X(503, SERVICE_UNAVAILABLE, Service Unavailable)                           \
    X(505, HTTP_VERSION_NOT_SUPPORTED, HTTP Version Not Supported)
//! Body for fallback 500 response, in case any unrecoverable error occurs
#define SERVER_ERROR_BODY                                                      \
    "<!DOCTYPE html>"                                                          \
    "<html lang=\"en\">"                                                       \
    "<head>"                                                                   \
    "<link rel=\"icon\" href=\"data:,\">"                                      \
    "<meta charset=\"utf-8\">"                                                 \
    "<title>500 Internal Server Error</title>"                                 \
    "</head>"                                                                  \
    "<body>"                                                                   \
    "<h1>500 Internal Server Error</h1>"                                       \
    "</body>"                                                                  \
    "</html>"
//! Complete fallback 500 response
#define FALLBACK_500_RESPONSE                                                  \
    "HTTP/1.1 500 Internal Server Error\r\n"                                   \
    "Content-Length: 191\r\n"                                                  \
    "Content-Type: text/html\r\n\r\n" SERVER_ERROR_BODY
//! ANSI control sequence to reset foreground color.
#define ANSI_RESET "\x1b[0m"
//! ANSI color code for red.
#define ANSI_RED "\x1b[0;31m"
//! ANSI color code for yellow.
#define ANSI_YELLOW "\x1b[0;33m"
//! ANSI color code for green.
#define ANSI_GREEN "\x1b[0;32m"
//! ANSI color code for blue.
#define ANSI_BLUE "\x1b[0;34m"
//! Various output macros
#define pr_error(format, ...)                                                  \
    (fprintf(stderr, ANSI_RED "[ERROR] (%s:%d) %s:" format ANSI_RESET "\n",      \
             __FILE__, __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__))
#ifdef NDEBUG
#define pr_warning(format, ...) ((void)0)
#define pr_info(format, ...) ((void)0)
#else
#define pr_warning(format, ...)                                                \
    (fprintf(stderr, ANSI_YELLOW "[WARN] (%s:%d) %s:" format ANSI_RESET "\n",    \
             __FILE__, __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__))
#define pr_info(format, ...)                                                   \
    (fprintf(stderr, ANSI_GREEN "[INFO] (%s:%d) %s:" format ANSI_RESET "\n",     \
             __FILE__, __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__))
#endif

#define FILE_PAIR 10
#define FOLDER_PAIR 20

#define CTRL_KEY(k) ((k) & 0x1f)

#define TEXTEDITOR_TAB_STOP 8

#define TEXTEDITOR_QUIT_TIMES 3

#endif // DEFINES_H
