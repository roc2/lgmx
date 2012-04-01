class view

{
    view *parent;

    src_container *src;

    QStackedLayout layout;

    view_manager* view_a;
    view_manager* view_b;

    QSplitter *splitter;

    bool splitted;
    bool is_root;
};

split::split()
{
    _src = src;
    _parent = parent;    
    splitter = false;

    set_layout(src);
}

void split(orientation)
{
    if (!split) {
        a = new view(this, src);
        b = new view(this, src);

        splitter.add(a);
        splitter.add(b);

        set_layout(splitter);
        split = true;
    }
}




