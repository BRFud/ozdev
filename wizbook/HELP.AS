FILEBUF_LEN equ 7592
global _filebuf,_dohelp,_background,__ozfile1buffer
global _frommemo_buf
global set_a000_400,restore_a000
global set_save_pagenum
global $copybuffer
global _uparrow,_downarrow
global _mygetch
global _arrow_1,_arrow_2

psect lowpage
_uparrow:
        ld      hl,0a000h+29
_arrow_1 equ $-2
        ld      de,30
arrow_do:
        call    set_a000_400 ;; only affects accumulator
        ld      a,0010000B
        ld      (hl),a
        add     hl,de
        ld      a,0111000B
        ld      (hl),a
        add     hl,de
        ld      a,1111100B
        ld      (hl),a
        jp      restore_a000
_downarrow:
        ld      hl,0a000h+2400-1
_arrow_2 equ $-2
        ld      de,-30
        jr      arrow_do


global _long_names_offset,_list_of_chapters_offset,_number_of_books
psect beforebss
__ozfile1buffer:
        defs 64
_long_names_offset equ $
_list_of_chapters_offset equ $+2
_number_of_books equ $+4
_filebuf:
defs 1984-64
_frommemo_buf:
defs 2000
;; this must be at least 2000 bytes, as it is
backdata:
*include backdata.inc
*include frommemo.inc
psect beforebss
$copybuffer:
defs 256
defs FILEBUF_LEN-($-_filebuf)
defb 0

_background:
        ld      hl,backdata
copy0:
        ld      de,0a000h
        call    set_a000_400
        ld      bc,2400
        ldir
        jp      restore_a000

_dohelp:
        ld      hl,0a968h
        call    copy0
        jp      _mygetch
