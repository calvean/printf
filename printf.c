#include "main.h"

/**
 * handle_specifiers - Matches a conversion specifier with
 *                     a corresponding conversion function.
 * @int: int
 * @char: char
 * Return: If a conversion function is matched - a pointer to the function.
 *         Otherwise - NULL.
 */
unsigned int (*handle_specifiers(const char *specifier))(va_list, buffer_t *,
		unsigned char, int, int, unsigned char)
{
	int i;
	converter_t converters[] = {
		{'c', convert_c},
		{'s', convert_s},
		{'d', convert_di},
		{'i', convert_di},
		{'%', convert_percent},
		{'b', convert_b},
		{'u', convert_u},
		{'o', convert_o},
		{'x', convert_x},
		{'X', convert_X},
		{'S', convert_S},
		{'p', convert_p},
		{'r', convert_r},
		{'R', convert_R},
		{0, NULL}
	};

	for (i = 0; converters[i].func; i++)
	{
		if (converters[i].specifier == *specifier)
			return (converters[i].func);
	}

	return (NULL);
}

/**
 * cleanup - Peforms cleanup operations for _printf.
 * @valist: A va_list of arguments provided to _printf.
 * @output: A buffer_t struct.
 */
void cleanup(va_list valist, buffer_t *output)
{
	va_end(valist);
	write(1, output->start, output->len);
	free_buffer(output);
}


/**
 * run_printf - Reads through the format string for _printf.
 * @format: Character string to print - may contain directives.
 * @output: A buffer_t struct containing a buffer.
 * @valist: A va_list of arguments.
 *
 * Return: The number of characters stored to output.
 */
int run_printf(const char *format, va_list valist, buffer_t *output)
{
	int i, wid, prec, ret = 0;
	char tmp;
	unsigned char flags, len;
/**
 * int - function
 * @f: function
 *
 * Return: int
 */
	unsigned int (*f)(va_list, buffer_t *,
			unsigned char, int, int, unsigned char);
	for (i = 0; *(format + i); i++)
	{
		len = 0;
		if (*(format + i) == '%')
		{
			tmp = 0;
			flags = handle_flags(format + i + 1, &tmp);
			wid = handle_width(valist, format + i + tmp + 1, &tmp);
			prec = handle_precision(valist, format + i + tmp + 1, &tmp);
			len = handle_length(format + i + tmp + 1, &tmp);
			f = handle_specifiers(format + i + tmp + 1);
			if (f != NULL)
			{
				i += tmp + 1;
				ret += f(valist, output, flags, wid, prec, len);
				continue;
			}
			else if (*(format + i + tmp + 1) == '\0')
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (format + i), 1);
		i += (len != 0) ? 1 : 0;
	}
	cleanup(valist, output);
	return (ret);
}

/**
 * _printf - prints anything
 * @format: argument types passed to the function
 *
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
	buffer_t *output;
	va_list valist;
	int ret;

	if (format == NULL)
		return (-1);
	output = init_buffer();
	if (output == NULL)
		return (-1);

	va_start(valist, format);

	ret = run_printf(format, valist, output);

	return (ret);
}
