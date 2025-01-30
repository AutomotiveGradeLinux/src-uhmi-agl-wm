#ifndef AGL_SHELL_GRPC_WRAPPER
#define AGL_SHELL_GRPC_WRAPPER

struct GrpcClient;

struct GrpcClient *
init_grpc_client(void);

void
grpc_client_set_app_float(struct GrpcClient *client, const char *app_id, int32_t x_pos, int32_t y_pos);

void
grpc_client_set_app_scale(struct GrpcClient *client, const char *app_id, int32_t width, int32_t heigth);

void
grpc_client_activate_app(struct GrpcClient *client, const char *app_id, const char *output);

char **
grpc_client_get_outputs(struct GrpcClient *client, int *output_no);

void
destroy_grpc_client(struct GrpcClient *client);

#endif
