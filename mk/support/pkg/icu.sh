
version=54.1

src_url=http://download.icu-project.org/files/icu4c/$version/icu4c-${version/\./_}-src.tgz

pkg_configure () {
    in_dir "$build_dir/source" ./configure --prefix="$(niceabspath "$install_dir")" --enable-static "$@"
}

pkg_make () {
    in_dir "$build_dir/source" make "$@"
}

pkg_install-include () {
    # pkg_configure
    # The install-headers-recursive target is missing. Let's patch it.
    sed 's/distclean-recursive/install-headers-recursive/g;$a\install-headers-local:' -i "$build_dir/source/Makefile"
    sed '$a\install-headers:' -i "$build_dir"/source/*/Makefile
    pkg_make install-headers-recursive
}
