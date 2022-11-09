import htmlmin

# TODO: Remove/Uninstall htmlmin, and use a commandline minifier tool. https://www.gribble.org/techreports/minification/

print("Hello world!")

with open('index.html', 'r') as file:
    data = file.read()
    print(data)
    minified = htmlmin.minify(data,
        remove_comments=True,
        remove_empty_space=True,
        remove_all_empty_space=True,
        reduce_empty_attributes=True,
        reduce_boolean_attributes=True
    )

    print()
    print()

    print(minified)


