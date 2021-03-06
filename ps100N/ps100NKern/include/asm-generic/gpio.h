_PORT_STAT_CONNECTION;
		if (status & USBPORTSC_PE) {
			wPortStatus |= USB_PORT_STAT_ENABLE;
			if (status & SUSPEND_BITS)
				wPortStatus |= USB_PORT_STAT_SUSPEND;
		}
		if (status & USBPORTSC_OC)
			wPortStatus |= USB_PORT_STAT_OVERCURRENT;
		if (status & USBPORTSC_PR)
			wPortStatus |= USB_PORT_STAT_RESET;
		if (status & USBPORTSC_LSDA)
			wPortStatus |= USB_PORT_STAT_LOW_SPEED;

		if (wPortChange)
			dev_dbg(uhci_dev(uhci), "port %d portsc %04x,%02x\n",
					wIndex, status, lstatus);

		*(__le16 *)buf = cpu_to_le16(wPortStatus);
		*(__le16 *)(buf + 2) = cpu_to_le16(wPortChange);
		OK(4);
	case SetHubFeature:		/* We don't implement these */
	case ClearHubFeature:
		switch (wValue) {
		case C_HUB_OVER_CURRENT:
		case C_HUB_LOCAL_POWER:
			OK(0);
		default:
			goto err;
		}
		break;
	case SetPortFeature:
		if (port >= uhci->rh_numports)
			goto err;

		switch (wValue) {
		case USB_PORT_FEAT_SUSPEND:
			SET_RH_PORTSTAT(USBPORTSC_SUSP);
			OK(0);
		case USB_PORT_FEAT_RESET:
			SET_RH_PORTSTAT(USBPORTSC_PR);

			/* Reset terminates Resume signalling */
			uhci_finish_suspend(uhci, port, port_addr);

			/* USB v2.0 7.1.7.5 */
			uhci->ports_timeout = jiffies + msecs_to_jiffies(50);
			OK(0);
		case USB_PORT_FEAT_POWER:
			/* UHCI has no power switching */
			OK(0);
		default:
			goto err;
		}
		break;
	case ClearPortFeature:
		if (port >= uhci->rh_numports)
			goto err;

		switch (wValue) {
		case USB_PORT_FEAT_ENABLE:
			CLR_RH_PORTSTAT(USBPORTSC_PE);

			/* Disable terminates Resume signalling */
			uhci_finish_suspend(uhci, port, port_addr);
			OK(0);
		case USB_PORT_FEAT_C_ENABLE:
			CLR_RH_PORTSTAT(USBPORTSC_PEC);
			OK(0);
		case USB_PORT_FEAT_SUSPEND:
			if (!(inw(port_addr) & USBPORTSC_SUSP)) {

				/* Make certain the port isn't suspended */
				uhci_finish_suspend(uhci, port, port_addr);
			} else if (!test_and_set_bit(port,
						&uhci->resuming_ports)) {
				SET_RH_PORTSTAT(USBPORTSC_RD);

				/* The controller won't allow RD to be set
				 * if the port is disabled.  When this happens
				 * just skip the Resume signalling.
				 */
				if (!(inw(port_addr) & USBPORTSC_RD))
					uhci_finish_suspend(uhci, port,
							port_addr);
				else
					/* USB v2.0 7.1.7.7 */
					uhci->ports_timeout = jiffies +
						msecs_to_jiffies(20);
			}
			OK(0);
		case USB_PORT_FEAT_C_SUSPEND:
			clear_bit(port, &uhci->port_c_suspend);
			OK(0);
		case USB_PORT_FEAT_POWER:
			/* UHCI has no power switching */
			goto err;
		case USB_PORT_FEAT_C_CONNECTION:
			CLR_RH_PORTSTAT(USBPORTSC_CSC);
			OK(0);
		case USB_PORT_FEAT_C_OVER_CURRENT:
			CLR_RH_PORTSTAT(USBPORTSC_OCC);
			OK(0);
		case USB_PORT_FEAT_C_RESET:
			/* this driver won't report these */
			OK(0);
		default:
			goto err;
		}
		break;
	case GetHubDescriptor:
		len = min_t(unsigned int, sizeof(root_hub_hub_des), wLength);
		memcpy(buf, root_hub_hub_des, len);
		if (len > 2)
			buf[2] = uhci->rh_numports;
		OK(len);
	default:
err:
		retval = -EPIPE;
	}
	spin_unlock_irqrestore(&uhci->lock, flags);

	return retval;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  