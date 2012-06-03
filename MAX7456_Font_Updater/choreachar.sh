#!/bin/sh

if [[ "$#" -ne 4 ]]; then
	echo "Uso correcto:" >&2
	echo " $0 ARCHIVO_ORIGEN POSICION_ORIGEN ARCHIVO_DESTINO POSICION DESTINO" >&2
	echo >&2
	echo " Ej: $0 orig.mcm 0xca dst.mcm 0xa" >&2
	exit 1
fi

FL_SRC=$1
CH_SRC=$2
FL_DST=$3
CH_DST=$4

tmp_dst="$3.tmp"


src_pos=`printf "%d" $2`
let src_head=(src_pos+1)*64+1
dst_pos=`printf "%d" $4`
let dst_head=dst_pos*64+1
let dst_tail=16385-dst_head-64

head -$dst_head "$FL_DST" > "$tmp_dst"

head -$src_head "$FL_SRC" | tail -64 >> "$tmp_dst"

tail -$dst_tail "$FL_DST" >> "$tmp_dst"

mv "$FL_DST" "$FL_DST.save" 
mv "$tmp_dst" "$FL_DST"


