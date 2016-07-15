# SFGUI contribution guidelines

## Introduction

This file gives information about some code guidelines for the SFGUI project.
The main reason is to keep the code style and behaviour of the whole library as
consistent as possible.

## Commits/contributions to the repository

The repository uses two special branches:

  * **master:** Includes commits that will definitely be in the next version.
  * **dev:** Includes commits that are most likely accepted to be merged to the
    master branch. Might be unstable/rewritten.

Send your patches through GitHub pull requests.

A real advantage for us is if you rebase your commits onto our latest commits
of the dev branch. This minimizes the merge effort we might have to spend when
applying your patch, and it makes sure that your changes work with our latest
changes.

## Code style

Check the sources of SFGUI to see how code is formatted. Formatting code is
like a religion for programmers, but please stick to SFGUI's style if you're
about to contribute source code.

## General rules

  1. **No definitions in class declarations:** Do not define something within a
     class declaration, including methods: Even if they're short, do not
     auto-inline by defining it.

     NO:

     ```
     class Foo {
       void Bar() { ... }
     };
     ```

     YES:

     ```
     class Foo {
       void Bar();
     };

     void Foo::Bar() {
       ...
     }
     ```

  2. **Pay attention to const-correct method signatures:** Getters mostly
     return const references — an exception are built-in C++ types like `int`,
     `char` etc.

     NO:

     ```
     sf::String GetText();
     int GetNumber();
     ```

     YES:

     ```
     const sf::String& GetText() const;
     int GetNumber() const;
     ```

  3. **Use descriptive and verbose names**, especially for methods.

     NO:

     ```
     void SetPos();
     for( int i = 0; i < 10; ++i ) {...}
     ```

     YES:

     ```
     void SetPosition();
     for( int widget_index = 0; widget_index < 10; ++widget_index ) {...}
     ```

  4. **Always use new-style casts.** Avoid `reinterpret_cast` and `const_cast`
     whenever possible.

     NO:

     ```
     float foo = (float)4;
     ```

     YES:

     ```
     float foo = static_cast<float>( 4 );
     ```
