" vim:set ts=8 sts=2 sw=2 tw=0:
"
" vimspeak.vim - Speaking text with MS Agent
"
" Author:	Yasuhiro Matsumoto <mattn_jp@hotmail.com>
" Last Change:	Fri, 27 Jun 2003

function! VimSpeak() range
  " Concatenate input string.
  let curline = a:firstline
  let strline = ''
  while curline <= a:lastline
    let tmpline = getline(curline)
    if tmpline=~ '\m^\a' && strline =~ '\m\a$'
      let strline = strline .' '. tmpline
    else
      let strline = strline . tmpline
    endif
    let curline = curline + 1
  endwhile
  call libcall("VimSpeak.dll", "Speak", strline)
endfunction

command! -nargs=0 -range= VimSpeak <line1>,<line2>call VimSpeak()

