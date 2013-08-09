var slidenum = 0;

function slideshow(position, update) {
	console.log('slideshow slidenum: ' + slidenum);
	switch (position) {
		case 0:
			//reset the patient record, increase slidemum and go to first slide
			startSlideShow();
			break;
		case 1: 
		case 2: 
		case 4: 
		case 6: 
		case 8: 
		case 10: 
		case 12: 
		case 14: 
		case 15: 
		case 17: 
		case 19:
		case 21:
			//go to the slide
			showSlide(position);
			break;
		case 3: 
		case 5: 
		case 7: 
		case 9: 
		case 11: 
		case 13:
		case 20:
			//go to the system
			hideSlideShow();
			break;
		case 16:
			//get cholesterol test and go to the system
			if (!update) {
				demoAction($('#demomenu li[data-cp-testtype="cholesterol"]'));
			}
			hideSlideShow();
			break;
		case 18:
			//get cholesterol test and go to the system
			if (!update) {
				demoAction($('#demomenu li[data-cp-testtype="glucose"]'));
			}
			hideSlideShow();
			break;
		default:
			//end slide show: disable buttons, hide the slideshow and reset slidenum to 0
			hideSlideShow();
			break;
	}
}

function displaySlideShow() {
	$('#slideshow').show();
}

function hideSlideShow() {
	$('#slideshow').hide();
}

function startSlideShow() {
	updatePatientRecord('001677980', '', 'reset');
	slidenum = 1;
	//$("#navbuttons #nextbtn").attr('disabled', 'false');
	showSlide(slidenum);
}

function showSlide(num) {
	console.log('showSlide slidenum: ' + slidenum);
	if (slidenum == 1) {
		//$("#navbuttons #previousbtn").attr('disabled', 'true');
	} else {
		//$("#navbuttons #previousbtn").attr('disabled', 'false');
	}	
	$('#slideshow').children('div').hide();
	var slide = "slide" + num;
	$('#' + slide).show();
	displaySlideShow();
}

function setSlidesBackground() {
	
	$('#slideshow').children('div.slide').each(function() {
		console.log("setSlidesBackground: " + 'url(images/slides/' + $(this).attr('id') + '.jpg) no-repeat');
		$(this).css('background', 'url(images/slides/' + $(this).attr('id') + '.jpg) no-repeat');
	});
}



// Wait for the DOM to be loaded. 
$(document).ready(function() { 
	
	setSlidesBackground();
	
	$("#navbuttons #startbtn").click(function(){
		startSlideShow();
	});
	
	$("#navbuttons #previousbtn").click(function(){
		slidenum--;
		slideshow(slidenum, true);
	});
	
	$("#navbuttons #nextbtn").click(function(){
		slidenum++;
		slideshow(slidenum, false);
	});
	
	$("#slideshow div.slide button.hint").click(function(){
		$(this).parents('div.hint').children('p').toggle();
	});
	
}); 