TEMPLATE = subdirs

SUBDIRS = \
        src \
        test

# NOTE: test/ has a dependency on src/ in the form of `georgerepo.pri`
