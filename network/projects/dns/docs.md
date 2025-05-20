# 1.2.2  Robustness Principle
[RFC 1123](https://www.rfc-editor.org/rfc/rfc1123.html#page-7)

## 1
> "Be liberal in what you accept, and
   conservative in what you send"

> Software should be written to deal with every conceivable
         error, no matter how unlikely; sooner or later a packet will
         come in with that particular combination of errors and
         attributes, and unless the software is prepared, chaos can
         ensue. 

> In general, it is best to assume that the network is
         filled with malevolent entities that will send in packets
         designed to have the worst possible effect.  This assumption
         will lead to suitable protective design, although the most
         serious problems in the Internet have been caused by
         unenvisaged mechanisms triggered by low-probability events;
         mere human malice would never have taken so devious a course!

- Adaptability to change must be designed into all levels of Internet host software

## 2
> The second part of the principle is almost as important:
         software on other hosts may contain deficiencies that make it
         unwise to exploit legal but obscure protocol features.  It is
         unwise to stray far from the obvious and simple, lest untoward
         effects result elsewhere.  A corollary of this is "watch out for misbehaving hosts"; host software should be prepared, not
         just to survive other misbehaving hosts, but also to cooperate
         to limit the amount of disruption such hosts can cause to the
         shared communication facility.

# 1.2.3  Error Logging

> Problem diagnosis will be aided if host implementations include
         a carefully designed facility for logging erroneous or
         "strange" protocol events.

> There is a tendency for abnormal but harmless protocol events
         to overflow error logging files; this can be avoided by using a
         "circular" log, or by enabling logging only while diagnosing a
         known failure. (It may be useful to filter and count duplicate
         successive messages)

# Strategy
> One strategy that seems to work well is:
         (1) always count abnormalities and make such counts accessible
         through the management protocol (see Section 6.3); and (2)
         allow the logging of a great variety of events to be
         selectively enabled.  For example, it might useful to be able
         to "log everything" or to "log everything for host X"