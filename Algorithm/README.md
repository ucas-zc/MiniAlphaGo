"""
本目录下pyx联合编译说明
"""
1.首先，需要安装Cython
2.编译需要依赖于setup.py
(ext_modules=cythonize('valid.pyx')后面的pyx文件名需要与待编译文件对应)
3.当前目录下执行python setyp.py build_ext --inplace进行编译，编译会
根据valid.pyx生成相应的valid.c文件
注意：最后，我们看一下valid.c文件最前面的注释
/* BEGIN: Cython Metadata
{
    "distutils": {
        "name": "valid",
        "sources": [
            "valid.pyx"
        ]
    },
    "module_name": "valid"
}
END: Cython Metadata */
我们可以得知该文件由valid.pyx生成了valid.c文件，最后的模块名称为valid，
我们就可以像导入其他python包一样进行编译了。