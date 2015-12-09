set nocompatible
filetype off

"git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

Plugin 'VundleVim/Vundle.vim'
Plugin 'tpope/vim-fugitive'
Plugin 'fatih/vim-go'

call vundle#end()
filetype plugin indent on

set nu!
set ts=4
set expandtab
set nowrap
set autoindent
set ruler
set fenc=utf-8