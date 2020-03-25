
if filereadable(expand("~/.vimrc.bundles"))
  source ~/.vimrc.bundles
endif

"""""一般设置""""""""""""""""""""""""""""""""""""""'""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set number	"显示行号
set syntax=on	"语法高亮
set ruler	"显示标尺
set nocompatible	"去掉vi的一致性模式
set lines=40 columns=150	"设置窗口大小
set tabstop=4	"tab键宽度
set softtabstop=4	"统一缩进4
set shiftwidth=4
set autoindent	"自动缩进
set cindent 
set smartindent
set noeb	"去掉错误提示音
set noexpandtab
set shortmess=atI	"启动时不显示援助乌干达儿童提示
set hlsearch		"搜索字符高亮
set incsearch
set iskeyword+=_,$,@,%,#,-	"这些字符不要被换行分割


""""""插件配置""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

filetype plugin on	"允许插件
if filereadable(expand("~/.vimrc.bundles"))
  source ~/.vimrc.bundles
endif

" Nerd Tree
" 文件树设置 {{{
" 关闭NERDTree快捷键
map <F3> :NERDTreeToggle<CR>
""当NERDTree为剩下的唯一窗口时自动关闭
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif
""修改树的显示图标
let g:NERDTreeDirArrowExpandable = '►'
let g:NERDTreeDirArrowCollapsible = '▼'
let NERDTreeAutoCenter=1
" 显示行号
let NERDTreeShowLineNumbers=1
" 是否显示隐藏文件
let NERDTreeShowHidden=1
" 设置宽度
let NERDTreeWinSize=25
" 在终端启动vim时，共享NERDTree
let g:nerdtree_tabs_open_on_console_startup=1
" 忽略一下文件的显示
let NERDTreeIgnore=['\.pyc','\~$','\.swp']
let g:NERDTreeIndicatorMapCustom = {
    \ "Modified"  : "✹",
    \ "Staged"    : "✚",
    \ "Untracked" : "✭",
    \ "Renamed"   : "➜",
    \ "Unmerged"  : "═",
    \ "Deleted"   : "✖",
    \ "Dirty"     : "✗",
    \ "Clean"     : "✔︎",
    \ 'Ignored'   : '☒',
    \ "Unknown"   : "?"
    \ }
" }}}

" Tagbar
let g:tagbar_ctags_bin='/usr/bin/ctags'
let g:tagbar_width=25
let g:tagbar_left=1
let g:tagbar_autofocus=1
nmap <F4> :TagbarToggle<CR>


"syntastic
"设置error和warning的标志
"let g:syntastic_mode_map = { 'mode': 'passive', 'active_filetypes': [],'passive_filetypes': [] }
"nnoremap <C-w> :SyntasticCheck<CR> :SyntasticToggleMode<CR>
let g:syntastic_enable_signs = 1
let g:syntastic_error_symbol='✗'
let g:syntastic_warning_symbol='►'
"总是打开Location List（相当于QuickFix）窗口，如果你发现syntastic因为与其他插件冲突而经常崩溃，将下面选项置0
let g:syntastic_always_populate_loc_list = 1
"自动打开Locaton List，默认值为2，表示发现错误时不自动打开，当修正以后没有再发现错误时自动关闭，
"设置1表示自动打开自动关闭，0表示关闭自动打开和自动关闭，3表示自动打开，但不自动关闭
let g:syntastic_auto_loc_list = 1
"修改Locaton List窗口高度
let g:syntastic_loc_list_height = 5
"打开文件时自动进行检查
let g:syntastic_check_on_open = 0
"自动跳转到发现的第一个错误或警告处
let g:syntastic_auto_jump = 1
"进行实时检查，如果觉得卡顿，将下面的选项置为1
let g:syntastic_check_on_wq = 0
"高亮错误
let g:syntastic_enable_highlighting=1
"让syntastic支持C++11
let g:syntastic_cpp_checkers = ['gcc']
let g:syntastic_cpp_compiler = 'gcc'
let g:syntastic_cpp_compiler_options = '-std=c++11'


"vim-easymotion
"忽略大小写
let g:EasyMotion_smartcase = 1

nmap s <Plug>(easymotion-overwin-f)
let g:EasyMotion_do_mapping = 0
nmap s <Plug>(easymotion-overwin-f2)
map <Leader>j <Plug>(easymotion-j)
map <Leader>k <Plug>(easymotion-k)

