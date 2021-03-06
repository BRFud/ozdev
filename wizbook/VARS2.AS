NUM_SINGLES equ 122  ;; must match wizbook.h
MAX_WORDS equ NUM_SINGLES+254*6
IDX_LEN equ 2*(MAX_WORDS+1)
global _dict_idx,_error_msg,_frommemo_table
global _fmerror_part,_fmerror_remains,_fmerror_unlink
global _liboz_upload,_fmerror_memoopen,_fmerror_v1,_fmerror_toolong
global _fmerror_openoutput,_fmerror_memo,_fmerror_writing
global _clear_msg,_sure_msg,_del_msg,_delall_msg,_nofiles_msg
global _bookmarks_msg,_dict_msg,_header_msg,_wizbook_header
global _none,_font_msg,_ext_msg,_zeros,_bm_dir
global _fheader,_hdr,_fm_filename,_fm_parthead
global _utoa_buffer,_checksum,_error_font
global _error_header,_entry_format
psect text
_error_msg:
        defm 'Error '
_dict_idx:
*include selectnm.aso
psect text
_files:
        defs 24*9
_frommemo_table:
        defs 203
_fmerror_part: defm 'Error: Cannot find part.'
               defb 0
_fmerror_remains:
               defm 'You may have the remains of another upload.'
               defb 0
_fmerror_unlink:
               defm 'Error unlinking. Continue?'
               defb 0
_liboz_upload:
               defm '**LIBOZ_UPLOAD*'
               defb 0
_fmerror_memoopen:
               defm 'Error opening memo.'
               defb 0
_fmerror_v1:
               defm 'V.1 file not supported.'
               defb 0
_fmerror_toolong:
               defm 'Filename too long.'
               defb 0
_fmerror_openoutput:
               defm 'Error opening output.'
               defb 0
_fmerror_memo:
              defm 'Error in memo.'
              defb 0
_fmerror_writing:
                defm 'Error writing.'
                defb 0
_del_msg:
        defm 'Deleting: '
        defb 0
_clear_msg:
        defm 'Clearing bookmarks for: '
        defb 0
_sure_msg:
        defm 'Are you sure?'
        defb 0
_delall_msg:
        defm 'Really delete all e-books?'
        defb 0
_nofiles_msg:
        defm 'No e-books found.'
        defb 0
_bookmarks_msg:
        defm '[bookmarks]'
        defb 0
_header_msg:
        defm '[header]'
        defb 0
_dict_msg:
        defm '[dictionary]'
        defb 0
_wizbook_header:
        defm 'WizBook'
        defb 0
_none:
        defm '(none)'
        defb 0
_font_msg:
        defm '[font]'
        defb 0
_ext_msg:
        defm '[ext]'
        defb 0
_zeros:
        defm '00000'
        defb 0
_bm_dir:
        defm 'bm/'
        defb 0
_fheader:
        defs 12
_hdr:
        defs 8
_fm_filename:
        defs 32
_fm_parthead:
        defs 21
_utoa_buffer:
        defb 0,0,0, 0,0,0
_checksum:
        defb 0,0,0, 0,0,0
_error_font:
        defm 'in font'
        defb 0
_error_header:
        defm 'in header'
        defb 0
_entry_format:
        defm '|. $'
        defb 0
psect text
defs IDX_LEN-($-_dict_idx)

                                                                                                
        
