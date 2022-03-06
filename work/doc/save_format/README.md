# A new save/transfer format for INR

## Introduction

The new save format:


        c = getc( fp );
    
    /* Magic Number */
    
        if ( c != 'I' ) { goto FAIL_FORMAT; }  c = getc( fp );
        if ( c != 'N' ) { goto FAIL_FORMAT; }  c = getc( fp );
        if ( c != 'R' ) { goto FAIL_FORMAT; }  c = getc( fp );
        if ( c != '2' ) { goto FAIL_FORMAT; }  c = getc( fp );
        if ( c != '1' ) { goto FAIL_FORMAT; }  c = getc( fp );
        if ( c != '0' ) { goto FAIL_FORMAT; }  c = getc( fp );
        if ( c != '\t' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Number of tapes */
    
        if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            number_tapes = c - '0'; c = getc( fp );
    
        while ( c != '\t' ) {
            if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            number_tapes = number_tapes * 10  +  ( c - '0' );
                c = getc( fp );
            if ( number_tapes >= MAXSHORT ) { goto FAIL_FORMAT; }
        }
        A-> A_nT = number_tapes;
        if ( c != '\t' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Number of rows */
    
        if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            number_rows = c - '0'; c = getc( fp );
    
        while ( c != '\n' ) {
            if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            number_rows = number_rows * 10  +  ( c - '0' );
                c = getc( fp );
            if ( number_rows >= MAXSHORT ) { goto FAIL_FORMAT; }
        }
        if ( c != '\n' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Get a row */
    
    NEXT_ROW:
    
    /* From state */
    
        if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            from_state = c - '0'; c = getc( fp );
    
        while ( c != '\t' ) {
            if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            from_state = from_state * 10  +  ( c - '0' );
                c = getc( fp );
            if ( from_state >= MAXSHORT ) { goto FAIL_FORMAT; }
        }
        if ( c != '\t' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* To state */
    
        if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            to_state = c - '0'; c = getc( fp );
    
        while ( c != '\t' ) {
            if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            to_state = to_state * 10  +  ( c - '0' );
                c = getc( fp );
            if ( to_state >= MAXSHORT ) { goto FAIL_FORMAT; }
        }
        if ( c != '\t' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Tape number */
    
        if ( c == '-' ) {
            c = getc( fp );
            if ( c != '1' ) { goto FAIL_FORMAT; } c = getc( fp );
            tape_no = (-1);
        } else {
            if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
                tape_no = c - '0'; c = getc( fp );
    
            while ( c != '\t' ) {
                if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
                tape_no = tape_no * 10  +  ( c - '0' );
                    c = getc( fp );
                if ( tape_no >= MAXSHORT ) { goto FAIL_FORMAT; }
            }
        }
        if ( c != '\t' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Token length */
    
        if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            length = c - '0'; c = getc( fp );
    
        while ( c != '\t' ) {
            if ( c < '0' || c > '9' ) { goto FAIL_FORMAT; }
            length = length * 10  +  ( c - '0' );
                c = getc( fp );
            if ( length >= MAXSHORT ) { goto FAIL_FORMAT; }
        }
        if ( c != '\t' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Get a token */
    
        if ( length + 1 >= Ssize( buffer ) ) {
            buffer = Srealloc( buffer, length + 1 );
        }
    
        i = 0;
        while ( i < length ) {
            buffer[ i++ ] = c;
            c = getc( fp );
            if ( c == EOF ) { goto FAIL_FORMAT; }
        }
        buffer[ i ] = '\0';
        if ( c != '\n' ) { goto FAIL_FORMAT; } c = getc( fp );
    
    /* Process a line here */
    
        if ( tape_no == -1 ) {
            A = A_add( A, from_state, 0, to_state );
            if ( length != 0 ) { goto FAIL_FORMAT; }
        } else {
            index = Tn_insert( Tn_Sigma, buffer, length );
            label = index * number_tapes + tape_no;
            A = A_add( A, from_state, label, to_state );
        }
    
        if ( c != EOF ) { goto NEXT_ROW; }
    
    /* Done: package up result */
    
        Sfree( buffer );
        return ( A );
    
    FAIL_FORMAT:


this is it.
