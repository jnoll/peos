/* 
 * Submit homework, in-class or via email.
 * $Id: submit_hw.pml,v 1.2 2004/07/16 01:43:01 jmontana Exp $
 */

process submit_document {
    action spell_check_document {
	requires { document }
	provides { document.spell_checked == "True" }
	script {
	    "Use your editor's spell check feature to verify the
correct spelling of your $document."
	}
    }

    action proofread_document {
	requires { document }
	provides { document }
	script {
	    "You cannot rely on automatic spell-checker's to catch
every spelling or typographic mistake.  Read your final formatted
$document carefully to be sure you have spelled each word correctly, that
punctuation and formatting are correct, and that you have not made
common mistakes such as substituting `their' for `there'."
	}
    }


    selection {
	action submit_hardcopy {
	    requires { document && 
		document.spell_checked == "True" && 
		document.proofread == "True"
	    }
	    script { "Print a copy of your $document and turn it in at the 
beginning of class." }
	}
	sequence {
	    action create_pdf_file {
		requires { document.spell_checked == "True" && 
		    document.proofread == "True" }
		provides { pdf_file }
		script {
		  "Create a PDF formatted version of your $document.  
If you are doing this on a Linux or Unix workstation and can export your 
document in PostScript form, use <i>ps2pdf</i>.  Otherwise, consult the 
documentation of your word processor."
		}
	    }
	    action submit_email {
		requires { pdf_file }
		provides { ack_message }
		script { 
		    "Create an email message with subject identifying
the course and assignment to which this document applies.  Attach the
$pdf_file to this message, ensuring the attachment is base64 encoded
and has type <i>application/pdf</i>.  Send a copy both to me and yourself."
		}
	    }
	    action verify_unpack {
		requires { ack_message }
		provides { unpacked_document }
		script { 
		    "Examine the $ack_message to be sure that: <ol>
<li> The attachment is included;</li> 
<li> The encoding is base64;</li> 
<li> The MIME type is <i>application/pdf</i>
<li> The $unpacked_document matches your original $document</li></ol>"
		}
	    }
	}
    }
}
