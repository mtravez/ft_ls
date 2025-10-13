#include "ft_ls.h"

t_flags get_flags(int argc, char **argv) {
    t_flags flags = {0};

    if (argc < 2) {
        return flags;
    }
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j = 1;
            while (argv[i][j]) {
                switch (argv[i][j])
                {
                    case 'l': flags.long_format = 1; break;
                        case 'a':
                        flags.all_files = 1;
                        break;
                        case 't':
                        flags.mod_time_order = 1;
                        break;
                        case 'r':
                        flags.reverse_order = 1;
                        break;
                        case 'R':
                        flags.display_recursive = 1;
                        break;
                    default:
                        ft_putstr_fd("ft_ls: unknown option -- ", STDERR_FILENO);
                        ft_putchar_fd(argv[i][j], STDERR_FILENO);
                        ft_putchar_fd('\n', STDERR_FILENO);
                        exit(EXIT_FAILURE);
                        break;
                }
                j++;
            }
        }
    }
    return flags;
}

void print_flags(t_flags flags) {
    ft_printf("all: %i\n long format: %i\n recursive: %i\n reverse: %i\n modification time: %i\n", flags.all_files, flags.long_format, flags.display_recursive, flags.reverse_order, flags.mod_time_order);
}
