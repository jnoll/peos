/* txt2PEOS is a minor modification of txt2pdbdoc. The only modification
 * is that DOC_CREATOR is now 'PEOS'
 */


/**	Text to Doc converter for 3Com PalmPilots
**	txt2pdbdoc.c
**
**	Copyright (C) 1998  Paul J. Lucas
**
**	This program is free software; you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation; either version 2 of the License, or
**	(at your option) any later version.
** 
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
** 
**	You should have received a copy of the GNU General Public License
**	along with this program; if not, write to the Free Software
**	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* standard */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 *
 *	Define integral type Byte, Word, and DWord to match those on the
 *	Pilot being 8, 16, and 32 bits, respectively.
 *
 *****************************************************************************/

#define U8_MAX		255u		/* max  8-bit unsigned */
#if	UCHAR_MAX == U8_MAX
typedef unsigned char	Byte;
#else
#error machine does not seem to support an 8-bit integral type
#endif

#define	U16_MAX		65535u		/* max 16-bit unsigned */
#if	USHRT_MAX == U16_MAX
typedef unsigned short	Word;
#else
#error machine does not seem to support a 16-bit integral type
#endif

#define	U32_MAX		4294967295ul	/* max 32-bit unsigned */
#if	USHRT_MAX == U32_MAX
typedef unsigned short	DWord;
#elif	UINT_MAX  == U32_MAX
typedef unsigned int	DWord;
#elif	ULONG_MAX == U32_MAX
typedef unsigned long	DWord;
#else
#error machine does not seem to support a 32-bit integral type
#endif

/********** You shouldn't have to change anything below this line. ***********/

#define bool		int
#define false		0
#define true		1

#define BUFFER_SIZE	6000	/* why this is this value, I don't know */
#define COUNT_BITS	3	/* ditto */
#define DISP_BITS	11	/* ditto */
#define	DOC_CREATOR	"PEOS"
#define	DOC_TYPE	"TEXt"
#define RECORD_SIZE_MAX	4096	/* Pilots have a fixed 4K record size */
#define VERSION		"1.2.1"

/*****************************************************************************
 *
 *	PDB file header
 *
 *****************************************************************************/

#define	dmDBNameLength	32	/* 31 chars + 1 null terminator */

typedef struct {		/* 78 bytes total */
	char	name[ dmDBNameLength ];
	Word	attributes;
	Word	version;
	DWord	create_time;
	DWord	modify_time;
	DWord	backup_time;
	DWord	modificationNumber;
	DWord	appInfoID;
	DWord	sortInfoID;
	char	type[4];
	char	creator[4];
	DWord	id_seed;
	DWord	nextRecordList;
	Word	numRecords;
} pdb_header;

/*
** Some compilers pad structures out to DWord boundaries so using sizeof()
** doesn't give the intended result.
*/
#define PDB_HEADER_SIZE		78
#define	PDB_RECORD_HEADER_SIZE	8

/*****************************************************************************
 *
 *	Doc record 0
 *
 *****************************************************************************/

typedef struct {		/* 16 bytes total */
	Word	version;	/* 1 = plain text, 2 = compressed */
	Word	reserved1;
	DWord	doc_size;	/* in bytes, when uncompressed */
	Word	numRecords; 	/* text rec's only; = pdb_header.numRecords-1 */
	Word	rec_size;	/* usually RECORD_SIZE_MAX */
	DWord	reserved2;
} doc_record0;

/*****************************************************************************
 *
 *	Globals
 *
 *****************************************************************************/

typedef struct {
	Byte	*buf;
	unsigned len;
} buffer;

char const*	me;				/* executable name */

bool		binary_opt = true;
bool		compress_opt = true;
bool		verbose_opt = false;
Word		(*swap_Word )(  Word );
DWord		(*swap_DWord)( DWord );

void		compress( buffer* );
void		decode( char const *src_file_name, char const *dest_file );
void		encode( char const *document_name,
			char const *src_file_name, char const *dest_file
		);
Byte*		mem_find( Byte *t, int t_len, Byte *m, int m_len );
FILE*		open_src_file ( char const *file_name );
FILE*		open_dest_file( char const *file_name );
void		put_byte( buffer*, Byte, bool *space );
void		remove_binary( buffer* );
void		select_swap();
Word		swap_Word_12( Word );
Word		swap_Word_21( Word );
DWord		swap_DWord_1234( DWord );
DWord		swap_DWord_4321( DWord );
void		uncompress( buffer* );
void		usage();
void		zero_fill( char*, int len );

#define NEW_BUFFER(b)	(b)->buf = malloc( (b)->len = BUFFER_SIZE )
#define GET_Word(f,n)	{ fread( &n, 2, 1, f ); n = swap_Word ( n ); }
#define GET_DWord(f,n)	{ fread( &n, 4, 1, f ); n = swap_DWord( n ); }
#define	PUT_Word(f,n)	{  Word n1 = swap_Word ( n ); fwrite( &n1, 2, 1, f ); }
#define	PUT_DWord(f,n)	{ DWord n1 = swap_DWord( n ); fwrite( &n1, 4, 1, f ); }

/*****************************************************************************
 *
 * SYNOPSIS
 */
	int main( int argc, char *argv[] )
/*
 * DESCRIPTION
 *
 *	Parse the command line, initialize, call other functions ... the
 *	usual things that are done in main().
 *
 * PARAMETERS
 *
 *	argc	The number of arguments.
 *
 *	argv	A vector of the arguments; argv[argc] is null.  Aside from
 *		the options below, the arguments are the names of the files.
 *
 * SEE ALSO
 *
 *	Brian W. Kernighan, Dennis M. Ritchie.  "The C Programming Language,
 *	2nd ed."  Addison-Wesley, Reading, MA.  pp. 114-118.
 *
 *****************************************************************************/
{
	extern char*	optarg;
	extern int	optind, opterr;

	bool		decode_opt = false;
	char const	opts[] = "bcdvV";	/* command line options */
	int		opt;			/* option being processed */

	me = strrchr( argv[0], '/' );		/* determine base name... */
	me = me ? me + 1 : argv[0];		/* ...of executable */

	select_swap();

	/********** Process command-line options *****************************/

	opterr = 1;
	while ( (opt = getopt( argc, argv, opts )) != EOF )
		switch ( opt ) {

			case 'b':
				binary_opt = false;
				break;

			case 'c':
				compress_opt = false;
				break;

			case 'd':
				decode_opt = true;
				break;

			case 'v':
				verbose_opt = true;
				break;

			case 'V':
				printf( "txt2pdbdoc %s\n", VERSION );
				exit( 0 );

			case '?': /* Bad option. */
				usage();
		}
	argc -= optind, argv += optind;

	if ( decode_opt ) {
		if ( argc < 1 || argc > 2 ) usage();
		decode( argv[ 0 ], argc == 2 ? argv[ 1 ] : 0 );
	} else {
		if ( argc != 3 ) usage();
		encode( argv[ 0 ], argv[ 1 ], argv[ 2 ] );
	}

	exit( 0 );
}

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void compress( buffer *b )
/*
 * DESCRIPTION
 *
 *	Replace the given buffer with a compressed version of itself.
 *
 * PARAMETERS
 *
 *	b	The buffer to be compressed.
 *
 *****************************************************************************/
{
	int i, j;
	bool space = false;

	Byte *buf_orig;
	Byte *p;	/* walking test hit; works up on successive matches */
	Byte *p_prev;
	Byte *head;	/* current test string */
	Byte *tail;	/* 1 past the current test buffer */
	Byte *end;	/* 1 past the end of the input buffer */

	p = p_prev = head = buf_orig = b->buf;
	tail = head + 1;
	end = b->buf + b->len;

	NEW_BUFFER( b );
	b->len = 0;

	/* loop, absorbing one more char from the input buffer on each pass */
	while ( head != end ) {
		/* establish where the scan can begin */
		if ( head - p_prev > (( 1 << DISP_BITS )-1) )
			p_prev = head - (( 1 << DISP_BITS )-1);

		/* scan in the previous data for a match */
		p = mem_find( p_prev, tail - p_prev, head, tail - head );

		/* on a mismatch or end of buffer, issued codes */
		if ( !p || p == head || tail - head > ( 1 << COUNT_BITS ) + 2
			|| tail == end
		) {
			/* issued the codes */
			/* first, check for short runs */
			if ( tail - head < 4 )
				put_byte( b, *head++, &space );
			else {
				unsigned dist = head - p_prev;
				unsigned compound = (dist << COUNT_BITS)
					+ tail - head - 4;

				if ( dist >= ( 1 << DISP_BITS ) ||
					tail - head - 4 > 7
				)
					fprintf( stderr,
						"%s: error: dist overflow\n", me
					);

				/* for longer runs, issue a run-code */
				/* issue space char if required */
				if ( space )
					b->buf[ b->len++ ] = ' ', space = false;

				b->buf[ b->len++ ] = 0x80 + ( compound >> 8 );
				b->buf[ b->len++ ] = compound & 0xFF;
				head = tail - 1;/* and start again */
			}
			p_prev = buf_orig;	/* start search again */
		} else
			p_prev = p;		/* got a match */

		/* when we get to the end of the buffer, don't inc past the */
		/* end; this forces the residue chars out one at a time */
		if ( tail != end )
			++tail;
	}
	free( buf_orig );

	if ( space )
		b->buf[ b->len++ ] = ' ';	/* add left-over space */

	/* final scan to merge consecutive high chars together */
	for ( i = j = 0; i < b->len; ++i, ++j ) {
		b->buf[ j ] = b->buf[ i ];

		/* skip run-length codes */
		if ( b->buf[ j ] >= 0x80 && b->buf[ j ] < 0xC0 )
			b->buf[ ++j ] = b->buf[ ++i ];

		/* if we hit a high char marker, look ahead for another */
		else if ( b->buf[ j ] == '\1' ) {
			b->buf[ j + 1 ] = b->buf[ i + 1 ];
			while ( i + 2 < b->len &&
				b->buf[ i + 2 ] == 1 && b->buf[ j ] < 8
			) {
				b->buf[ j ]++;
				b->buf[ j + b->buf[ j ] ] = b->buf[ i + 3 ];
				i += 2;
			}
			j += b->buf[ j ];
			++i;
		}
	}
	b->len = j;
}

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void decode( char const *src_file_name, char const *dest_file_name )
/*
 * DESCRIPTION
 *
 *	Decode the source Doc file to a text file.
 *
 * PARAMETERS
 *
 *	src_file_name	The name of the Doc file.
 *
 *	dest_file_name	The name of the text file.  If null, text is sent to
 *			standard output.
 *
 *****************************************************************************/
{
	FILE		*fin, *fout;
	pdb_header	header;
	doc_record0	rec0;
	int		compression;
	int		num_records, rec_num;
	buffer		buf;
	DWord		file_size, offset;

	/********** open files; ensure source is a Doc file ******************/

	fin = open_src_file( src_file_name );

	fread( &header, PDB_HEADER_SIZE, 1, fin );
	if ( strncmp( header.type,    DOC_TYPE,    sizeof header.type ) ||
	     strncmp( header.creator, DOC_CREATOR, sizeof header.creator )
	) {
		fprintf( stderr, "%s: %s is not a Doc file\n",
			me, dest_file_name
		);
		exit( 3 );
	}
	num_records = swap_Word( header.numRecords ) - 1; /* not inc'l rec 0 */

	fout = dest_file_name ? open_dest_file( dest_file_name ) : stdout;

	/********** read record 0 ********************************************/

	fseek( fin, PDB_HEADER_SIZE, SEEK_SET );/* start of index */
	GET_DWord( fin, offset );		/* get offset of rec 0 */
	fseek( fin, offset, SEEK_SET );
	fread( &rec0, sizeof rec0, 1, fin );

	compression = swap_Word( rec0.version );
	if ( compression != 1 && compression != 2 ) {
		fprintf( stderr,
			"%s: error: unknown file compression type: %d\n",
			me, compression
		);
		exit( 3 );
	}

	/********* read buffer ***********************************************/

	fseek( fin, 0, SEEK_END );
	file_size = ftell( fin );

	if ( verbose_opt )
		fprintf( stderr, "%s: decoding \"%s\":", me, header.name );

	NEW_BUFFER( &buf );
	for ( rec_num = 1; rec_num <= num_records; ++rec_num ) {
		DWord next_offset;

		/* read the record offset */
		fseek( fin, PDB_HEADER_SIZE +
			PDB_RECORD_HEADER_SIZE * rec_num, SEEK_SET
		);
		GET_DWord( fin, offset );
		if ( rec_num < num_records ) {
			fseek( fin, PDB_HEADER_SIZE +
				PDB_RECORD_HEADER_SIZE * (rec_num + 1), SEEK_SET
			);
			GET_DWord( fin, next_offset );
		} else
			next_offset = file_size;

		fseek( fin, offset, SEEK_SET );
		buf.len = fread( buf.buf, 1, next_offset - offset, fin );

		if ( compression == 2 )
			uncompress( &buf );

		fwrite( buf.buf, buf.len, 1, fout );
		if ( verbose_opt )
			fprintf( stderr, " %d", num_records - rec_num - 1 );
	}
	if ( verbose_opt )
		putc( '\n', stderr );

	fclose( fin );
	fclose( fout );
}

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void encode( char const *document_name,
		char const *src_file_name, char const *dest_file_name
	)
/*
 * DESCRIPTION
 *
 *	Encode the source text file into a Doc file.
 *
 * PARAMETERS
 *
 *	document_name	The name of the document as it is to appear in the
 *			Documents List view of a Doc reader application on
 *			the Pilot.
 *
 *	src_file_name	The name of the text file.
 *
 *	dest_file_name	The name of the Doc file.
 *
 *****************************************************************************/
{
	FILE		*fin, *fout;
	DWord		file_size;
	pdb_header	header;
	doc_record0	rec0;
	buffer		buf;
	int		num_records, rec_num;
	DWord		num_offsets, offset;
	unsigned long	index;
	int		total_before, total_after;

	fin  = open_src_file ( src_file_name  );
	fout = open_dest_file( dest_file_name );

	fseek( fin, 0, SEEK_END );
	file_size = ftell( fin );
	fseek( fin, 0, SEEK_SET );
	num_records = file_size / RECORD_SIZE_MAX;
	if ( (long)num_records * RECORD_SIZE_MAX < file_size )
		++num_records;

	/********** create and write header **********************************/

	zero_fill( header.name, sizeof header.name );
	strncpy( header.name, document_name, sizeof header.name - 1 );
	if ( strlen( document_name ) > sizeof header.name - 1 )
		strncpy( header.name + sizeof header.name - 4, "...", 3 );
	header.attributes		= 0;
	header.version			= 0;
	strncpy( (char*)&header.create_time, "\x06\xD1\x44\xAE", 4 );
	strncpy( (char*)&header.modify_time, "\x06\xD1\x44\xAE", 4 );
	header.backup_time		= 0;
	header.modificationNumber	= 0;
	header.appInfoID		= 0;
	header.sortInfoID		= 0;
	strncpy( header.type,    DOC_TYPE,    sizeof header.type );
	strncpy( header.creator, DOC_CREATOR, sizeof header.creator );
	header.id_seed			= 0;
	header.nextRecordList		= 0;
	header.numRecords		= swap_Word( num_records + 1 );
						/* +1 for rec 0 */

	fwrite( &header, PDB_HEADER_SIZE, 1, fout );

	/********** write record offsets *************************************/

	num_offsets = num_records + 1;		/* +1 for rec 0 */
	offset = PDB_HEADER_SIZE + PDB_RECORD_HEADER_SIZE * num_offsets;
	index = 0x40 << 24 | 0x6F8000;		/* dirty + unique ID */

	PUT_DWord( fout, offset );		/* offset for rec 0 */
	PUT_DWord( fout, index++ );

	while( --num_offsets ) {
		PUT_DWord( fout, 0 );		/* placeholder */
		PUT_DWord( fout, index++ );
	}

	/********** write record 0 *******************************************/

	rec0.version	= swap_Word( compress_opt + 1 );
	rec0.reserved1	= 0;
	rec0.doc_size	= swap_DWord( file_size );
	rec0.numRecords	= swap_Word( num_records );
	rec0.rec_size	= swap_Word( RECORD_SIZE_MAX );
	rec0.reserved2	= 0;

	fwrite( &rec0, sizeof rec0, 1, fout );

	/********** write text ***********************************************/

	NEW_BUFFER( &buf );
	total_before = total_after = 0;
	for ( rec_num = 1; rec_num <= num_records; ++rec_num ) {
		int bytes_read;

		offset = ftell( fout );
		fseek( fout, PDB_HEADER_SIZE +
			PDB_RECORD_HEADER_SIZE * rec_num, SEEK_SET
		);
		PUT_DWord( fout, offset );

		if ( !(bytes_read = fread( buf.buf, 1, RECORD_SIZE_MAX, fin )) )
			break;
		buf.len = bytes_read;

		if ( binary_opt )
			remove_binary( &buf );
		if ( compress_opt )
			compress( &buf );

		fseek( fout, offset, SEEK_SET );
		fwrite( buf.buf, buf.len, 1, fout );

		if ( !verbose_opt )
			continue;

		if ( compress_opt ) {
			fprintf( stderr,
				"  record %2d: %5d bytes -> %5d (%2d%%)\n",
				rec_num, bytes_read, buf.len,
				(int)( 100.0 * buf.len / bytes_read )
			);
			total_before += bytes_read;
			total_after  += buf.len;
		} else
			fprintf( stderr, " %d", num_records - rec_num + 1 );
	}
	if ( verbose_opt )
		if ( compress_opt )
			fprintf( stderr, "\
                                  -----\n\
	        total compression: %2d%%\n",
				(int)( 100.0 * total_after / total_before )
			);
		else
			putc( '\n', stderr );

	fclose( fin );
	fclose( fout );
}

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void put_byte( register buffer *b, Byte c, bool *space )
/*
 * DESCRIPTION
 *
 *	Put a byte into a buffer.
 *
 * PARAMETERS
 *
 *	b	The buffer to be affected.
 *
 *	c	The byte.
 *
 *	space	Is it a space?
 *
 *****************************************************************************/
{
	if ( *space ) {
		*space = false;
		/*
		** There is an outstanding space char: see if we can squeeze it
		** in with an ASCII char.
		*/
		if ( c >= 0x40 && c <= 0x7F ) {
			b->buf[ b->len++ ] = c ^ 0x80;
			return;
		}
		b->buf[ b->len++ ] = ' ';	/* couldn't squeeze it in */
	} else if ( c == ' ' ) {
		*space = true;
		return;
	}

	if ( c >= 1 && c <= 8 || c >= 0x80 )
		b->buf[ b->len++ ] = '\1';

	b->buf[ b->len++ ] = c;
}

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void remove_binary( register buffer *b )
/*
 * DESCRIPTION
 *
 *	Replace the given buffer with one that has had ASCII characters (0-8)
 *	removed and carriage-returns and form-feeds converted to newlines.
 *
 * PARAMETERS
 *
 *	b	The buffer to be affected.
 *
 *****************************************************************************/
{
	register Byte *const new_buf = malloc( b->len );
	int i, j;
	for ( i = j = 0; i < b->len; ++i ) {
		if ( b->buf[ i ] < 9 )		/* discard really low ASCII */
			continue;
		switch ( b->buf[ i ] ) {

			case '\r':
				if ( i < b->len - 1 && b->buf[ i + 1 ] == '\n' )
					continue;	/* CR+LF -> LF */
				/* no break; */

			case '\f':
				new_buf[ j ] = '\n';
				break;

			default:
				new_buf[ j ] = b->buf[ i ];
		}
		++j;
	}
	free( b->buf );
	b->buf = new_buf;
	b->len = j;
}

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void select_swap()
/*
 * DESCRIPTION
 *
 *	Determine the "endianness" of the machine by copying byte strings
 *	into a union and seeing how they come out when looked at as integers;
 *	set pointers-to-function appropriately.
 *
 *****************************************************************************/
{
	union { char c[2];  Word n; }  w;
	union { char c[4]; DWord n; } dw;

	strncpy(  w.c, "\1\2",     2 );
	strncpy( dw.c, "\1\2\3\4", 4 );

	swap_Word  =	 w.n == 0x0102 ? swap_Word_12 :
			 w.n == 0x0201 ? swap_Word_21 : 0;

	swap_DWord =	dw.n == 0x01020304 ? swap_DWord_1234 :
			dw.n == 0x04030201 ? swap_DWord_4321 : 0;

	if ( swap_Word && swap_DWord )
		return;

	fprintf( stderr, "%s: couldn't select byte-swapping algorithm\n", me );
	exit( 2 );
}  

/*****************************************************************************
 *
 * SYNOPSIS
 */
	void uncompress( register buffer *b )
/*
 * DESCRIPTION
 *
 *	Replace the given buffer with an uncompressed version of itself.
 *
 * PARAMETERS
 *
 *	b	The buffer to be uncompressed.
 *
 *****************************************************************************/
{
	Byte *const new_buf = malloc( BUFFER_SIZE );
	int i, j;
	for ( i = j = 0; i < b->len; ) {
		register unsigned c = b->buf[ i++ ];

		if ( c >= 1 && c <= 8 )
			while ( c-- )			/* copy 'c' bytes */
				new_buf[ j++ ] = b->buf[ i++ ];

		else if ( c <= 0x7F )			/* 0,09-7F = self */
			new_buf[ j++ ] = c;

		else if ( c >= 0xC0 )			/* space + ASCII char */
			new_buf[ j++ ] = ' ', new_buf[ j++ ] = c ^ 0x80;

		else {					/* 80-BF = sequences */
			register int di, n;
			c = (c << 8) + b->buf[ i++ ];
			di = (c & 0x3FFF) >> COUNT_BITS;
			for ( n = (c & ((1 << COUNT_BITS) - 1)) + 3; n--; ++j )
				new_buf[ j ] = new_buf[ j - di ];
		}
	}
	free( b->buf );
	b->buf = new_buf;
	b->len = j;
}

/*****************************************************************************
 *
 *	Miscellaneous function(s)
 *
 *****************************************************************************/

/* replacement for strstr() which deals with 0's in the data */
Byte* mem_find( register Byte *t, int t_len, register Byte *m, int m_len ) {
	register int i;
	for ( i = t_len - m_len + 1; i > 0; --i, ++t )
		if ( *t == *m && !memcmp( t, m, m_len ) )
			return t;
	return 0;
}

FILE* open_src_file( char const *file_name ) {
	FILE *f = fopen( file_name, "rb" );
	if ( f ) return f;
	fprintf( stderr, "%s: can not open %s for input\n", me, file_name );
	exit( 2 );
}

FILE* open_dest_file( char const *file_name ) {
	FILE *f = fopen( file_name, "wb" );
	if ( f ) return f;
	fprintf( stderr, "%s: can not open %s for output\n", me, file_name );
	exit( 2 );
}

Word swap_Word_12( Word r ) {
	return r;  
}

Word swap_Word_21( Word r ) {
	return (r >> 8) | (r << 8);
}

DWord swap_DWord_1234( DWord r ) {
	return r;
}

DWord swap_DWord_4321( DWord r ) {
	return	( (r >> 24) & 0x00FF ) |   (r << 24) |
		( (r >>  8) & 0xFF00 ) | ( (r <<  8) & 0xFF0000 );
}

void usage() {
	fprintf( stderr, "\
usage: %s [-c] [-b] [-v] document-name file.txt file.pdb\n\
       %s  -d  [-b] [-v] file.pdb [file.txt]\n\n\
 options:\n\
 --------\n\
  -b: don't strip binary characters\n\
  -c: don't compress Doc file\n\
  -d: decode Doc file to text\n\
  -v: verbose\n", me, me );
	exit( 1 );
}

void zero_fill( register char *p, register int len ) {
	while ( len-- > 0 )
		*p++ = '\0';
}
